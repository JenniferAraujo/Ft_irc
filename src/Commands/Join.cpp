#include "Includes.hpp"

Join::Join(const Server& server, const Client& client): ACommand("JOIN", server, client) {};

void Join::parsing(std::istringstream &input){
	std::string channel;
    std::getline(input, channel, '\n');
    this->trimChar(channel, '\r');
    channel.erase(0, 1);
    this->_channel = channel;
}
