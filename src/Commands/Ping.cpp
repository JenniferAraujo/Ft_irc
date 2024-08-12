#include "Includes.hpp"

Ping::Ping(Server& server, Client& client): ACommand("PING", server, client) {};

void Ping::parsing(std::istringstream &input){
	std::string token;
    std::getline(input, token, '\n');
    this->trimChar(token, '\r');
    std::cout << "token: " << token << std::endl;
    this->_token = token;
}

void Ping::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << this->_name << std::endl;
    std::string msg;
    msg.append(PONG(this->_server.getHostname(), this->_token));
    send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
}