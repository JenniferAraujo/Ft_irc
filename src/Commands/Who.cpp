#include "Includes.hpp"

Who::Who(const Server& server, const Client& client): ACommand("WHO", server, client) {};

void Who::parsing(std::istringstream &input){
	std::string channel;
    std::getline(input, channel, '\n');
    this->trimChar(channel, '\r');
    channel.erase(0, 1);
    this->_channel = channel;
	 std::cout << "channel:" << this->_channel << std::endl;
}
