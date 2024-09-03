#include "Includes.hpp"

Invite::Invite(Server& server, Client& client): ACommand("INVITE", server, client) {};

void Invite::parsing(std::istringstream &input) {
    // USERONCHANNEL 443
    // CHANOPRIVNEEDED 482
	std::string token;
    int n = 0;
    while (std::getline(input, token, ' ') || n < 1 ) {
        this->trimChar(token, '\r');
        if (token.empty()) {
            this->_error = NEEDMOREPARAMS;      //INVITE  dinoguei
            return;
        }
        switch (n) {
            case 0:
                if (this->existentClient(token))
                    this->_nickname = token;      //INVITE dinoguei #a
                else {
                    this->_error = NOSUCHNICK;
                    return ;
                }
                break;
            case 1:
                if (this->existentChannel(token))
                    this->_channel = token;     //INVITE dinoguei #a
                else {
                    this->_error = NOSUCHCHANNEL;
                    return ;
                }
        }
        n++;
    }
    if (this->_nickname.empty() || this->_channel.empty())
		this->_error = NEEDMOREPARAMS;
}

void Invite::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << this->_name << std::endl;
}

void Invite::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << " | Nickname: " << this->_nickname << " | Channel: " << this->_channel << std::endl;
}