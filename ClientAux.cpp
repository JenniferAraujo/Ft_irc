#include "Includes.hpp"

int Client::getSocketFD() const{
    return this->_socketFD;
}

sockaddr_in6 Client::getSocketInfo() const {
    return this->_socketInfo;
}

std::string Client::getNick() const{
    return this->_nick;
}

std::string Client::getName() const{
    return this->_name;
}

std::string Client::getRealName() const{
    return this->_realName;
}