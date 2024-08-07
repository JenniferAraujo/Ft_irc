#include "Includes.hpp"

Ping::Ping(const Server& server, const Client& client): ACommand("PING", server, client) {};

void Ping::parsing(std::istringstream &input){
	std::string token;
    std::getline(input, token, '\n');
    this->trimChar(token, '\r');
    std::cout << "token: " << token << std::endl;
    this->_token = token;
}
