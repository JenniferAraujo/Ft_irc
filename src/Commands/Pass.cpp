#include "Includes.hpp"

Pass::Pass(const Server& server, const Client& client): ACommand("PASS", server, client) {};

void Pass::parsing(std::istringstream &input){
    std::getline(input, this->_pass, '\r');
    if(_server.getPassword() != this->_pass)
        this->_error = PASSWDMISMATCH;
}

Pass::~Pass() {}