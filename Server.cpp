#include "Includes.hpp"

Server::Server()
{
}

Server::Server(const int &port, const std::string &password) : _port(port),
                                                               _password(password)
{
    memset(&this->_socketInfo, 0, sizeof(this->_socketInfo));
    _socketInfo.sin6_family = AF_INET6; // IPv6
    _socketInfo.sin6_port = htons(this->_port);
    _socketInfo.sin6_addr = in6addr_any;
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

//*Proximos passos: canais e mansagens
//Ideia para executar os cmds
void    Server::executeCommand(Client &client){
    //std::string commands[N] = {"JOIN", "MSG", ...}
    //void (Server::*p[])(const Client&) = {&Server::join, &Server::msg};
/*     for (int i = 0; i < N; i++) {
        if(!commands[i].compare(client.getCommand())){
            (this->*p[i])(client);
    } */
    std::cout << "Command to execute " << client.getCommand() << RESET <<  std::endl;
}

// Função para verificar eventos na poll
//Verificar it->revents == POLLIN
//Verificar it-> revents == POLLOUT
//Isto so itera pelos clientes, se o fd do evento for o do cliente, verifica se é IN ou OUT e printa uma msg
void Server::verifyEvent(const pollfd &pfd) {
    for(std::map<int, Client*>::iterator it = _Clients.begin(); it != _Clients.end(); ++it){
        Client *client = it->second;
        if(pfd.fd == it->first){
            if(pfd.revents == POLLIN) {
                std::cout << "Event on client " << GREEN << "[" << pfd.fd << "]" << RESET <<  std::endl;
                std::vector<char> buf(5000);
                int bytes = recv(client->getSocketFD(), buf.data(), buf.size(), 0);
                std::cout << "N Bytes received: " << bytes << std::endl;
                std::cout << buf.data() << "." << std::endl;
                client->parseMessage(buf);
                if(client->getValidCmd() == true){
                    this->executeCommand(*client);
                }
                std::cout << "Client's info saved: " << *client  << std::endl;
            }
            if(pfd.events == POLLOUT)
                std::cout << "pollout event" << *client  << std::endl;
        }
    }
    printMap(_Clients);
}

// Função para verificar que evento aconteceu
void Server::checkEvents(int nEvents) {
    (void)nEvents;
    std::vector<pollfd> NFDs2 = this->_NFDs;
    std::cout << "CheckEvents: " << std::endl;
    for (std::vector<pollfd>::iterator it = NFDs2.begin(); it != NFDs2.end(); ++it)
    {
        // Se for no fd da socket é pk é uma nova conecção, caso contrário, alguém enviou dados
        std::cout << *it;
        (it->fd == this->_socketFD)
            ? Client::verifyConnection(*this, *it)
            : this->verifyEvent(*it);
    }
    // Deixem isto só para ser mais fácil analisar
    usleep(10000000);
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
    std::cout << "Socket with fd " << GREEN "[" << this->_socketFD << "]" << RESET
              << " bound on port " << YELLOW << this->_port << RESET << std::endl;
    // Meter a socket a ouvir aquela porta para um máximo de X conecções
    if (listen(this->_socketFD, 10) == -1)
    {
        close(this->_socketFD);
        throw IRCException("[ERROR] Listening socket went wrong");
    }
    std::cout << "Server listening only " << YELLOW << 10 << RESET << " connections\n"
              << std::endl;
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
            break;
        }
    }
    close(this->_socketFD);
}

Server::~Server() {
    for (std::map<int, Client*>::iterator it = _Clients.begin(); it != _Clients.end(); ++it) {
        delete it->second;
    }
}
