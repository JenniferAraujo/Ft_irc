#include "Includes.hpp"

Quit::Quit(Server& server, Client& client): ACommand("QUIT", server, client){};

void Quit::parsing(std::istringstream &input){
    std::string str;
    std::getline(input, str, ':');
    if(std::cin.eof()){
        _error = NEEDMOREPARAMS; //nao tem a formataçao ":reason"
        return ;
    }
    std::getline(input, this->_reason);
    trimChar(this->_reason, '\r');
}

void Quit::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << this->_name << std::endl;
    if(this->_error)
        Message::sendMessage(this->_client.getSocketFD(), ERR_NEEDMOREPARAMS(this->_server.getHostIP(), this->_client.getNick(), this->_name),this->_server);
    this->_server.updateToRemove(this->_client.getSocketFD(), "Quit: " + this->_reason);
}

void Quit::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << " | Error: " << this->_reason << std::endl;
}