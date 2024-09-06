#include "Includes.hpp"

Pass::Pass(Server& server, Client& client): ACommand("PASS", server, client) {};

void Pass::parsing(std::istringstream &input){
    std::getline(input, this->_pass, ' ');
    if (this->_pass.empty())
        this->_error = NEEDMOREPARAMS; //PASS
    else{
        trimChar(this->_pass, '\r');
        if(_server.getPassword() != this->_pass)
            this->_error = PASSWDMISMATCH; //PASS wrongpass
    }
}

void Pass::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << RESET << this->_name << std::endl;
    if (this->_error == NEEDMOREPARAMS){
        Message::sendMessage(this->_client.getSocketFD(), ERR_NEEDMOREPARAMS(this->_server.getHostname(), this->_client.getNick(), this->_name), this->_server);
        return ;
    }
    if(this->_client.getRegistration()){
        Message::sendMessage(this->_client.getSocketFD(), ERR_ALREADYREGISTERED(this->_server.getHostname(), this->_client.getNick()), this->_server);
        return ;
    }
    if (this->_error) {
        this->_client.setRegError(true);
    }
    else{
        this->_client.setRegError(0);
        this->_client.setPass(this->_pass);
    }
}

void Pass::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << " | Pass: " << this->_pass<< std::endl;
}