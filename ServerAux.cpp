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