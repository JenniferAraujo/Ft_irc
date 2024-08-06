#include "Includes.hpp"

Who::Who(const Server& server, const Client& client): ACommand("Who", server, client) {};

void Who::parsing(std::istringstream &input){
	std::string channel;
    std::getline(input, channel, '\r');
    channel.erase(0, 1);
    this->_channel = channel;
}
