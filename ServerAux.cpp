#include "Includes.hpp"

int Server::getSocketFD() {
    return this->_socketFD;
}

int Server::getPort() {
    return this->_port;
}

sockaddr_in6 Server::getSocketInfo() {
    return this->_socketInfo;
}