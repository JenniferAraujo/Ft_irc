#include "Includes.hpp"

Ping::Ping(Server& server, Client& client): ACommand("PING", server, client) {};

void Ping::parsing(std::istringstream &input){
    std::string token;
    int n = 0;
    while (std::getline(input, token, ' ') || n < 1) {
        trimChar(token, '\r');
        if (token.empty()) {
            this->_error = NEEDMOREPARAMS;          //PING  1234567890
            return;
        }
        switch (n)
        {
            case 0: //PING 1234567890
                this->_token = token;
                if (this->_token.empty()) {
                    this->_error = NEEDMOREPARAMS;
                    return;
                }
        }
        n++;
    }
}

void Ping::execute() {
    //std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << this->_name;
    //this->print();
    switch (this->_error) {
        case NEEDMOREPARAMS:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NEEDMOREPARAMS(this->_server.getHostname(), this->_client.getNick(), this->_name), this->_server);
            break;
        default:
            Message::sendMessage(this->_client.getSocketFD(), PONG(this->_server.getHostname(), this->_token), this->_server);
            break;
    }
}

void Ping::print() const{
    if (this->_error != 0)
        std::cout << " " << RED << "[" << this->_error << "]" << std::endl;
    else
        std::cout << "\t[" << this->_token << "]" << std::endl;
}