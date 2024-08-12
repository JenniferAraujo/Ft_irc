#include "Includes.hpp"

Pass::Pass(Server& server, Client& client): ACommand("PASS", server, client) {};

void Pass::parsing(std::istringstream &input){
    std::getline(input, this->_pass, '\n');
    this->trimChar(this->_pass, '\r');
    if(_server.getPassword() != this->_pass)
        this->_error = PASSWDMISMATCH;
}

void Pass::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << RESET << this->_name << std::endl;
    std::string msg;
    if(this->_client.getRegistration()){
        msg.append(ERROR("You may not reregister"));
        send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
        return ;
    }
    if (this->_error) {
        msg.append(ERROR("Password incorrect"));
        send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
        //this->_toRemove.push_back(this->_client.getSocketFD()); //TODO create function Server::addToRemove
        this->_client.setAuthError(PASSWDMISMATCH);
    }
    else
        this->_client.setPass(this->_pass);
}