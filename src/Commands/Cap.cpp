#include "Includes.hpp"

Cap::Cap(Server& server, Client& client): ACommand("CAP", server, client){};

void Cap::parsing(std::istringstream &input){
	std::string str;
    std::getline(input, str, ' ');
    if (str.empty())
        this->_error = NEEDMOREPARAMS; //CAP
    else {
        trimChar(str, '\r');
        if(str != "LS")
            this->_error = UNKNOWNCOMMAND; //CAP dsjag
    }
}

void Cap::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << this->_name;
    this->print();
    std::string msg;
    switch (this->_error) {
        case NEEDMOREPARAMS:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NEEDMOREPARAMS(this->_server.getHostname(), this->_client.getNick(), this->_name), this->_server);
            break;
        case UNKNOWNCOMMAND:
            Message::sendMessage(this->_client.getSocketFD(), ERR_UNKNOWNCOMMAND(this->_server.getHostname(), this->_client.getNick(), this->_name), this->_server);
            break;
        default:
            if(this->_client.getRegistration())
                Message::sendMessage(this->_client.getSocketFD(), ERR_ALREADYREGISTERED(this->_server.getHostname(), this->_client.getNick()), this->_server);
            break;
    }
}

void Cap::print() const{
    //std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << std::endl;
    if (this->_error != 0)
        std::cout << " " << RED << "[" << this->_error << "]" << std::endl;
    else
        std::cout << std::endl;
}

//1o os clientes podem enviar
//CAP LS [version] or CAP REQ

//CAP EMD - COMPLETE REGISTRATION 