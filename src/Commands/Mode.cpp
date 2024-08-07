#include "Includes.hpp"

Mode::Mode(const Server& server, const Client& client): ACommand("MODE", server, client) {};

void Mode::parsing(std::istringstream &input){
	std::string channel;
    std::getline(input, channel, ' ');
    channel.erase(0, 1);
    this->_channel = channel;
    std::string str;
    std::getline(input, str, '\n');
    this->trimChar(str, '\r');
}
