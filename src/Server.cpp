#include "Includes.hpp"

Server::Server() {}

Server::Server(const int &port, const std::string &password) : _port(port),
															   _password(password)
{
	memset(&this->_socketInfo, 0, sizeof(this->_socketInfo));
	_socketInfo.sin6_family = AF_INET6;
	_socketInfo.sin6_port = htons(this->_port);
	_socketInfo.sin6_addr = in6addr_any;
	this->_creationTime = getCurrentDateTime();
}

Server::Server(const Server &cpy)
    :   _port(cpy.getPort()),
        _password(cpy.getPassword()),
        _hostName(cpy.getHostname()),
        _hostIP(cpy.getHostIP()),
        _socketFD(cpy.getSocketFD()),
        _creationTime(cpy.getCreationTime()),
        _socketInfo(cpy.getSocketInfo()),
        _NFDs(cpy.getNFD()),
        _Clients(cpy.getClients()),
        _Channels(cpy.getChannels()),
        _toRemove(cpy.getToRemove()) {}

void Server::updateNFDs(int fd)
{
	pollfd pfd = {fd, POLLIN, 0};
	this->_NFDs.push_back(pfd);
}

void Server::updateClients(Client *client, int fd) { this->_Clients[fd] = client; }

void Server::updateToRemove(int fd, std::string reason)
{
    this->_toRemove[fd] = reason;
}

/* ERR_NOTREGISTERED (451)

  "<client> :You have not registered"

Returned when a client command cannot be parsed as they are not yet registered. Servers offer only a limited subset of commands until clients are properly registered to the server. The text used in the last param of this message may vary.
 */

void    Server::executeCommand(Client &client, ACommand *command){
    //REVIEW - Nao sei se esta condição é necessaria -> if((!client.getRegistration() || client.getRegError()) && !command->isRegistration())
    if(!client.getRegistration() && !command->isRegistration())
            std::cout << RED << "ERROR: " << RESET
            << "Auth not over -> client can not execute command: " <<
            command->getName() << std::endl;
    else
        command->execute();
}

void    Server::handleCommand(Client &client, std::vector<char> &buf){
    //std::cout << "FINAL BUF: " << buf.data() << "." << std::endl;
    std::queue<ACommand *> commands = client.createCommand(buf);
    if (commands.empty()) //Nao e um comando/ comando que nao tratamos -> //TODO - erro de unknoncommmand
            return ;
    std::cout << BOLD_GREEN << "[PRINT COMMANDS]\n" << RESET;
    showq(commands);
    while(!commands.empty()){
        ACommand *command = commands.front();
        this->executeCommand(client, command);
        commands.pop();
        delete command;
    }
}

//TODO PRIVMSG -> handle send errors
//TODO QUIT -> tirar dos canais (KICK), remover client, mandar msg
//TODO handle size da msg -> ver qual o tamannho max que o server recebe/buffer overflow -> bytesReceived < 0 ??
void Server::verifyEvent(const pollfd &pfd) {
    if (pfd.revents == POLLIN) {
        Client *client = this->_Clients[pfd.fd];

        // Update the last activity time with the current time
        client->setLastActivityTime(time(NULL));

        //Most IRC servers limit messages to 512 bytes in length
        std::vector<char> temp(MAX_MESSAGE_SIZE + 1);
        static std::vector<char> buf;

        std::cout << formatServerMessage(BOLD_CYAN, "SERVER", this->_Clients.size()) << "Event on Client " << GREEN << "[" << client->getSocketFD() << "]" << RESET <<  std::endl;
        std::cout << formatServerMessage(BOLD_PURPLE, "C.INFO", 0) << *client << std::endl;

        int bytesReceived = recv(client->getSocketFD(), temp.data(), temp.size(), 0);
        if(bytesReceived == 0){
            this->updateToRemove(client->getSocketFD(), "Connection closed by client");
            return ;
        }
        if(bytesReceived < 0){
            if (errno != EWOULDBLOCK || errno != EAGAIN){
                this->updateToRemove(client->getSocketFD(), "Recv fail");
            }
            return ;
        }
        // Ensure the buffer does not overflow
        if(bytesReceived > MAX_MESSAGE_SIZE){
            Message::sendMessage(client->getSocketFD(), ERR_UNKNOWNERROR(this->_hostName, client->getNick(), "", "Buffer overflow detected"), *this);
            buf.clear();
            return;
        }
        //std::cout << "TEMP: " << temp.data() << "." << std::endl;

        // Ensure the buffer does not overflow
        if (buf.size() + bytesReceived > MAX_MESSAGE_SIZE) {
            Message::sendMessage(client->getSocketFD(), ERR_UNKNOWNERROR(this->_hostName, client->getNick(), "", "Buffer overflow detected"), *this);
            buf.clear();
            return;
        }
        // Insert received data into the buffer
        buf.insert(buf.end(), temp.begin(), temp.begin() + bytesReceived);
        //std::cout << "BUF: " << buf.data() << "." << std::endl;

        // If the message contains a newline, process it
        if (std::find(temp.begin(), temp.end(), '\n') != temp.end()){
            this->handleCommand(*client, buf);
            buf.clear();
            //std::cout << "BUF after clear: " << buf.data() << "." << std::endl;
            //std::cout << "BUF SIZE: " << buf.size() << std::endl;
        }
        // If the client isn't registered, try registration
        if (!client->getRegistration())
            client->registration();
    }
}

// Função para verificar que evento aconteceu
void Server::checkEvents(int nEvents) {
    (void)nEvents;
    std::vector<pollfd> NFDs2 = this->_NFDs;
    //titleInfo("Fds Vector");
    //std::cout << NFDs2;
    for (std::vector<pollfd>::iterator it = NFDs2.begin(); it != NFDs2.end(); ++it)
    {
        (it->fd == this->_socketFD)
            ? Client::verifyConnection(*this, *it)
            : this->verifyEvent(*it);
    }
    for (std::map <int, std::string>::iterator it = this->_toRemove.begin(); it != _toRemove.end(); ++it) {
        this->removeClient(it->first, it->second);
    }
    this->_toRemove.clear();
    std::cout << std::endl;
    //printMap(_Clients);
}

void Server::handleTimeouts(time_t inactivityTimeout, time_t connectionTimeout) {
    time_t now = time(NULL);

    for (std::map<int, Client *>::iterator it = this->_Clients.begin(); it != this->_Clients.end(); ++it) {
/*         std::cout << "Now: " << now << " | Last time activity: " << it->second->getLastActivityTime() 
        << "Dif: " << now - it->second->getLastActivityTime() << " | TIMEOUT: " << timeoutDuration << std::endl; */
        if (!it->second->getRegistration()){
            if(now - it->second->getConnectTime() >= connectionTimeout)
                this->updateToRemove(it->first, "Connection timeout");
        }
        else{
            if (now - it->second->getLastActivityTime() >= inactivityTimeout){
                this->updateToRemove(it->first, "Inactivity timeout");
            }
        }
    }
    for (std::map <int, std::string>::iterator it = this->_toRemove.begin(); it != _toRemove.end(); ++it) {
        this->removeClient(it->first, it->second);
    }
    this->_toRemove.clear();
}

void Server::run()
{
    // 1.Inicializa a socket e da exception se der erro
    this->_socketFD = socket(AF_INET6, SOCK_STREAM, 0);
    if (this->_socketFD == -1)
        throw IRCException("[ERROR] Opening socket went wrong");
    // 2. Set socket options (e.g., SO_REUSEADDR)
    int enable = 1;
    if (setsockopt(this->_socketFD, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1){
        close(this->_socketFD);
        throw IRCException("[ERROR] Setting socket options went wrong");
    }
    // 3. Set the socket to non-blocking mode
    int flags = fcntl(this->_socketFD, F_GETFL, 0);
    if (flags == -1) {
        close(this->_socketFD);
        throw IRCException("[ERROR] Getting socket flags went wrong");
    }
    if (fcntl(this->_socketFD, F_SETFL, flags | O_NONBLOCK) == -1) {
        close(this->_socketFD);
        throw IRCException("[ERROR] Setting socket to non-blocking went wrong");
    }
    // 4. Dá bind aquela mesma socket numa porta específica
    if (bind(this->_socketFD, reinterpret_cast<struct sockaddr *>(&this->_socketInfo), sizeof(this->_socketInfo)) == -1)
    {
        close(this->_socketFD);
        throw IRCException("[ERROR] Binding socket went wrong");
    }
    std::cout << BOLD_YELLOW << "[SERVER INFO]\t" << RESET << "Socket with fd " << GREEN "[" << this->_socketFD << "]" << RESET
              << " bound on port " << YELLOW << this->_port << RESET << std::endl;
    // Meter a socket a ouvir aquela porta para um máximo de X conecções
    //TODO - Testar too many connections
    if (listen(this->_socketFD, 10) == -1)
    {
        close(this->_socketFD);
        throw IRCException("[ERROR] Listening socket went wrong");
    }
    std::cout << BOLD_YELLOW << "[SERVER INFO]\t" << RESET << "Server listening only " << YELLOW << 10 << RESET << " connections"
              << std::endl;
    this->getServerInfo();
    // Adicionar o FD da socket aqueles que a poll vai poder monitorizar
    this->updateNFDs(this->_socketFD);
    // Ciclo para correr a poll para esperar eventos
    while (true)
    {
        switch (int nEvents = poll(this->_NFDs.data(), this->_NFDs.size(), TIMEOUT * 1000))
        {
        case -1:
            throw IRCException("[ERROR] Poll went wrong");
        case 0:
            this->handleTimeouts(TIMEOUT, CONNECTIONTIMEOUT);
            throw IRCException("[ERROR] Poll connection timed out");
        default:
            this->checkEvents(nEvents);
            this->handleTimeouts(TIMEOUT, CONNECTIONTIMEOUT);
            //titleInfo("Clients Map");
            //printMap(_Clients);
            break;
        }
    }
    close(this->_socketFD);
}

Server::~Server() {
	for (std::map<int, Client*>::iterator it = _Clients.begin(); it != _Clients.end(); ++it) {
		close(it->first);
		delete it->second;
	}
}
