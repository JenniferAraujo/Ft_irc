#include "Includes.hpp"

Mode::Mode(const Server& server, const Client& client): ACommand("Mode", server, client) {};

void Mode::parsing(std::istringstream &input){
	std::string channel;
    std::getline(input, channel, '\r');
    channel.erase(0, 1);
    this->_channel = channel;
}
