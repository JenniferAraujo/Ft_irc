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
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << this->_name;
    this->print();
    if (this->_error == NEEDMOREPARAMS){
        Message::sendMessage(this->_client.getSocketFD(), ERR_NEEDMOREPARAMS(this->_server.getHostname(), this->_client.getNick(), this->_name), this->_server);
        return ;
    }
    if (this->_error == PASSWDMISMATCH){
        Message::sendMessage(this->_client.getSocketFD(), ERR_PASSWDMISMATCH(this->_server.getHostname(), this->_client.getNick()), this->_server);
        this->_client.setRegError(true);
        return ;
    }
    if(this->_client.getRegistration()){
        Message::sendMessage(this->_client.getSocketFD(), ERR_ALREADYREGISTERED(this->_server.getHostname(), this->_client.getNick()), this->_server);
        return ;
    }
    this->_client.setRegError(0);
    this->_client.setPass(this->_pass);
}

void Pass::print() const{
    if (this->_error != 0)
        std::cout << " " << RED << "[" << this->_error << "]" << std::endl;
    else
        std::cout << "\t[" << this->_pass << "]" << std::endl; 
}