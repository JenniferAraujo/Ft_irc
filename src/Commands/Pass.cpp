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
    //TODO - validaçoes de erros parsing
    if(this->_client.getRegistration()){
        msg.append(ERR_ALREADYREGISTERED(this->_server.getHostname(), this->_client.getNick()));
        send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
        return ;
    }
    if (this->_error) {
        this->_client.setRegError(PASSWDMISMATCH);
    }
    else
        this->_client.setPass(this->_pass);
}

void Pass::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << " | Pass: " << this->_pass<< std::endl;
}