#include "Includes.hpp"

int Server::getSocketFD() const {
    return this->_socketFD;
}

int Server::getPort() const {
    return this->_port;
}

sockaddr_in6 Server::getSocketInfo() const {
    return this->_socketInfo;
}

void    Server::removeClient(int fd){
    if(this->_Clients.find(fd) != this->_Clients.end()){
        close(fd);
        this->_Clients.erase(fd);
    }
}