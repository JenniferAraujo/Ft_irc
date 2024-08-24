#include "Includes.hpp"

Cap::Cap(Server& server, Client& client): ACommand("CAP", server, client){};

void Cap::parsing(std::istringstream &input){
	std::string str;
    std::getline(input, str, ' ');
    if (str.empty())
        this->_error = NEEDMOREPARAMS; //CAP
    else {
        this->trimChar(str, '\r');
        if(str != "LS")
            this->_error = UNKNOWNCOMMAND; //CAP dsjag
    }
}

void Cap::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << this->_name << std::endl;
    std::string msg;
    switch (this->_error) {
        case NEEDMOREPARAMS:
            if (this->_client.getNick().empty())
                msg.append(ERR_NEEDMOREPARAMS(this->_server.getHostname(), "*", this->_name));
            else
                msg.append(ERR_NEEDMOREPARAMS(this->_server.getHostname(), this->_client.getNick(), this->_name));
            break;
        case UNKNOWNCOMMAND:
            if (this->_client.getNick().empty())
                msg.append(ERR_UNKNOWNCOMMAND(this->_server.getHostname(), "*" , this->_name));
            else
                msg.append(ERR_UNKNOWNCOMMAND(this->_server.getHostname(), this->_client.getNick(), this->_name));
            break;
        default:
            if(this->_client.getRegistration())
                    msg.append(ERR_ALREADYREGISTERED(this->_server.getHostname(), this->_client.getNick()));
            break;
    }
    send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
}

void Cap::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << std::endl;
}

//1o os clientes podem enviar
//CAP LS [version] or CAP REQ

//CAP EMD - COMPLETE REGISTRATION 