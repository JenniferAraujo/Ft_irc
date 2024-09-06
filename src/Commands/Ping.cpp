#include "Includes.hpp"

Ping::Ping(Server& server, Client& client): ACommand("PING", server, client) {};

void Ping::parsing(std::istringstream &input){
	std::string token;
    std::getline(input, token, '\n');
    trimChar(token, '\r');
    this->_token = token;
}

void Ping::execute() {
    //std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << this->_name << std::endl;
    Message::sendMessage(this->_client.getSocketFD(), PONG(this->_server.getHostname(), this->_token), this->_server);
}

void Ping::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << " | Token: " << this->_token << std::endl;
}