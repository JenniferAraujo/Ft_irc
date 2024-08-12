#include "Includes.hpp"

Cap::Cap(Server& server, Client& client): ACommand("CAP", server, client), _end(false){};

void Cap::parsing(std::istringstream &input){
	std::string str;
    std::getline(input, str, ' ');
    if(!str.compare("END\r"))
        this->_end = true;
    else if(str != "LS")
        this->_error = 1;
}

void Cap::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << this->_name << std::endl;
    std::string msg;
    if(this->_client.getRegistration() && !this->_end){
        msg.append(ERROR("You may not reregister"));
        send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
        return ;
    }
    if (!this->_end) {
        msg.append(CLIENT_NEGOTIATION(this->_server.getHostname()));
        send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
    }
}