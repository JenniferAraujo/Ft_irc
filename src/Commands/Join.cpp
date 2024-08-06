#include "Includes.hpp"

Join::Join(const Server& server, const Client& client): ACommand("Join", server, client) {};

void Join::parsing(std::istringstream &input){
	std::string channel;
    std::getline(input, channel, '\r');
    channel.erase(0, 1);
    this->_channel = channel;
}

Join::~Join() {}