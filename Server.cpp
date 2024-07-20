#include "Includes.hpp"

Server::Server()
{
    std::cout << BOLD_WHITE << "Server" << RESET
              << " default constructor called" << std::endl;
}

Server::Server(const int &port, const std::string &password) : _port(port),
                                                               _password(password)
{
    std::cout << BOLD_WHITE << "Server: " << RESET
              << "Port: " << _port
              << " | Pass: " << _password << std::endl;
}

void Server::run() {
    // Inicializa a socket e da exception se der erro
    this->_socketFD = socket(AF_INET6, SOCK_STREAM, 0);
    if (this->_socketFD == -1)
        throw IRCException("[ERROR] Opening socket went wrong");
    _socketInfo.sin6_family = AF_INET6;
    _socketInfo.sin6_port = htons(this->_port);
    _socketInfo.sin6_addr = in6addr_loopback;


    
    close(this->_socketFD);
}

Server::~Server() {}
