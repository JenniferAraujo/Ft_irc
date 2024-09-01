#include "Includes.hpp"

Quit::Quit(Server& server, Client& client): ACommand("CAP", server, client){};

void Quit::parsing(std::istringstream &input){
    std::getline(input, this->_reason, ' ');
    this->trimChar(this->_reason, '\r');
}

//mandar msg para todos os clientes??
void Quit::execute() {
    this->_server.updateToRemove(this->_client.getSocketFD(), "Quit: " + this->_reason);
}

void Quit::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << " | Error: " << this->_reason << std::endl;
}