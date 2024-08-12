#include "Includes.hpp"

Mode::Mode(Server& server, Client& client): ACommand("MODE", server, client) {};

void Mode::parsing(std::istringstream &input){
	std::string channel;
    std::getline(input, channel, ' ');
    channel.erase(0, 1);
    this->_channel = channel;
    std::string str;
    std::getline(input, str, '\n');
    this->trimChar(str, '\r');
}

void Mode::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << this->_name << std::endl;
    std::string msg;
    msg.append(RPL_MODE(this->_server.getHostname(), this->_channel, this->_client.getNick(), "+nt"));
    send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
}
