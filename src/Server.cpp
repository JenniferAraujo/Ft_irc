#include "Includes.hpp"

Server::Server()
{
}

Server::Server(const int &port, const std::string &password) : _port(port),
                                                               _password(password)
{
    memset(&this->_socketInfo, 0, sizeof(this->_socketInfo));
    _socketInfo.sin6_family = AF_INET6;
    _socketInfo.sin6_port = htons(this->_port);
    _socketInfo.sin6_addr = in6addr_any;
    this->_creationTime = getCurrentDateTime();
}

Server::Server(const Server &cpy): _port(cpy.getPort()), _password(cpy.getPassword())
{
}

// Função para adicionar fds para a poll monitorizar
void Server::updateNFDs(int fd)
{
    pollfd pfd = {fd, POLLIN, 0};
    this->_NFDs.push_back(pfd);
}

void Server::updateClients(Client *client, int fd)
{
    this->_Clients[fd] = client;
}

void Server::welcome(Client &client) {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << RESET << "WELCOME" << std::endl;
    std::string msg;
    if (client.getPassword().empty()) {
        msg.append(ERROR("No password was set"));
        send(client.getSocketFD(), msg.c_str(), msg.length(), 0);
        this->_toRemove.push_back(client.getSocketFD());
    } else {
        msg.append(RPL_WELCOME(this->_hostName, "Internet Fight Club", client.getNick(), client.getUsername(), client.getIpaddr()));
        msg.append(RPL_YOURHOST(this->_hostName, "servername", client.getNick(), "version"));
        msg.append(RPL_CREATED(this->_hostName, this->getCreationTime(), client.getNick()));
        msg.append(RPL_MYINFO(this->_hostName, client.getNick(), "servername"));
        msg.append(RPL_ISUPPORT(this->_hostName, client.getNick()));
        msg.append(RPL_MOTDSTART(this->_hostName, client.getNick(), "servername"));
        msg.append(RPL_MOTD(this->_hostName, "  ________________", client.getNick()));
        msg.append(RPL_MOTD(this->_hostName, " /______________ /|", client.getNick()));
        msg.append(RPL_MOTD(this->_hostName, "|  ___________  | |", client.getNick()));
        msg.append(RPL_MOTD(this->_hostName, "| |           | | |", client.getNick()));
        msg.append(RPL_MOTD(this->_hostName, "| |  ft_irc   | | |", client.getNick()));
        msg.append(RPL_MOTD(this->_hostName, "| |    by:    | | |", client.getNick()));
        msg.append(RPL_MOTD(this->_hostName, "| |           | | |", client.getNick()));
        msg.append(RPL_MOTD(this->_hostName, "| |  r, j, d  | | |", client.getNick()));
        msg.append(RPL_MOTD(this->_hostName, "| |___________| | |  ___", client.getNick()));
        msg.append(RPL_MOTD(this->_hostName, "|_______________|/  /  /", client.getNick()));
        msg.append(RPL_MOTD(this->_hostName, "                   /__/", client.getNick()));
        msg.append(RPL_ENDOFMOTD(this->_hostName, client.getNick()));
        send(client.getSocketFD(), msg.c_str(), msg.length(), 0);
    }
}


//*Proximos passos: canais e mensagens
//Ideia para executar os cmds
void    Server::executeCommand(Client &client, ACommand *command){
    if((!client.getRegistration() || client.getAuthError()) && !registration_command(command->getName()))
            std::cout << RED << "ERROR: " << RESET
            << "Auth not over or auth error -> client can not execute command: " <<
            command->getName() << ", and will be disconected soon" << std::endl;
    else
        command->execute();
}

void    Server::handleCommand(Client &client, std::vector<char> &buf){
    std::cout << "FINAL BUF: " << buf.data() << "." << std::endl;
    std::queue<ACommand *> commands = client.createCommand(buf);
    if (commands.empty()) //Nao e um comando/ comando que nao tratamos
            return ;
    std::cout << BOLD_GREEN << "[PRINT COMMANDS]\n" << RESET;
    showq(commands);
    while(!commands.empty()){
        ACommand *command = commands.front();
        this->executeCommand(client, command);
        commands.pop();
        delete command;
    }
    //std::cout << BOLD_GREEN << "[PRINT CLIENTS]\n" << RESET;
    //printMap(this->_Clients);
    if(!client.getRegistration() && !client.getAuthError() && !client.getNick().empty()
                && !client.getRealname().empty() && !client.getUsername().empty()){
                    //TODO welcome dependent on cap
                    //if(client.cap == false || (client.cap == true && client.capEnd == true)){
                    this->welcome(client);
                    client.setRegistration(true);
    }
    //TODO disconnection depending on cap
    /*if(client.capEnd == false && client.ping > 5){
    this->_toRemove.push_back(client.getSocketFD());
    client.setAuthError(INVALIDCAP);
    } */
    std::cout << std::endl;

}


//TODO QUIT -> fechar o fd
//TODO desconectar clientes
//TODO handle size da msg -> ver qual o tamannho max que o server recebe
void Server::verifyEvent(const pollfd &pfd) {
    if(pfd.revents == POLLIN) {
        Client *client = this->_Clients[pfd.fd];
        std::vector<char> temp(5000);
        static std::vector<char> buf(5000);
        std::cout << formatServerMessage(BOLD_CYAN, "SERVER", this->_Clients.size()) << "Event on Client " << GREEN << "[" << client->getSocketFD() << "]" << RESET <<  std::endl;
        std::cout << formatServerMessage(BOLD_PURPLE, "C.INFO", 0) << *client << std::endl;
        recv(client->getSocketFD(), temp.data(), temp.size(), 0);
        std::cout << "TEMP: " << temp.data() << "." << std::endl;
        std::vector<char>::iterator it = std::find(buf.begin(), buf.end(), '\0'); // Procurando por um caractere nulo (onde os dados atuais terminam)
        if (it == buf.end()) {
            // Se não houver '\0' no vetor, use o fim como posição inicial
            it = buf.begin(); 
        }
        if (std::find(temp.begin(), temp.end(), '\n') == temp.end()){
            buf.insert(it, temp.begin(), temp.end());
            //std::cout << "BUF: " << buf.data() << "." << std::endl;
        }
        else{
            buf.insert(it, temp.begin(), temp.end());
            this->handleCommand(*client, buf);
            buf.clear();
        }
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
    //for (std::vector<int>::iterator it = toRemove.begin(); it != toRemove.end(); ++it) {
    //    this->removeClient(*it);
    //}
}

void Server::run()
{
    // Inicializa a socket e da exception se der erro
    this->_socketFD = socket(AF_INET6, SOCK_STREAM, 0);
    if (this->_socketFD == -1)
        throw IRCException("[ERROR] Opening socket went wrong");
    int enable = 1;
    if (setsockopt(this->_socketFD, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1){
        close(this->_socketFD);
        throw IRCException("[ERROR] Setting socket options went wrong");
    }
    // Dá bind aquele mesma socket numa porta específica
    if (bind(this->_socketFD, reinterpret_cast<struct sockaddr *>(&this->_socketInfo), sizeof(this->_socketInfo)) == -1)
    {
        close(this->_socketFD);
        throw IRCException("[ERROR] Binding socket went wrong");
    }
    std::cout << BOLD_YELLOW << "[SERVER INFO]\t" << RESET << "Socket with fd " << GREEN "[" << this->_socketFD << "]" << RESET
              << " bound on port " << YELLOW << this->_port << RESET << std::endl;
    // Meter a socket a ouvir aquela porta para um máximo de X conecções
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
        switch (int nEvents = poll(this->_NFDs.data(), this->_NFDs.size(), -1))
        {
        case -1:
            throw IRCException("[ERROR] Poll went wrong");
        case 0:
            throw IRCException("[ERROR] Poll connection timed out");
        default:
            this->checkEvents(nEvents);
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
