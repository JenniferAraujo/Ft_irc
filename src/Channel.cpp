#include "Includes.hpp"

Channel::Channel(){}

Channel::Channel(std::string name): _name(name){}

//TODO copy

void Channel::addClient(Client &client) {
    this->_Clients[client.getSocketFD()] = &client;
}

Channel::~Channel() {}