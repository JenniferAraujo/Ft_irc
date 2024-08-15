#include "Includes.hpp"

Kick::Kick(Server& server, Client& client): ACommand("KICK", server, client) {};

void Kick::parsing(std::istringstream &input) {
	std::string token;
    int n = 0;
    while (std::getline(input, token, ' ') || n < 3) {
        this->trimChar(token, '\r');
        if (n == 0) {
            if (token[0] == '#') {
                if (this->existentChannel(token))
                    this->_channel = token;     //KICK #a Diogo
                else {
                    this->_error = NOSUCHCHANNEL;
                    return ;
                }
            }
            else {
                this->_error = BADCHANMASK;     //KICK a
                return ;
            }
        }
        else if (n == 1) {
            if (this->existentClient(token))
                this->_client = token;			//KICK #a Diogo
            else {
                this->_error = USERNOTINCHANNEL;
                return ;
            }
        }
        else if (n == 2) {
			if (token[0] == ':') {
            	token.erase(token.begin());
            	this->_reason = token;			//KICK #a Diogo :idk
			}
			else {
				this->_error = NEEDMOREPARAMS;	//KICK #a Diogo idk
				return ;
			}
        }
        n++;
    }
    if (this->_channel.empty() || this->_client.empty())
		this->_error = NEEDMOREPARAMS;
}

void Kick::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << this->_name << std::endl;
    std::string msg;
}

void Kick::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error;
    std::cout << " | Channel: " << this->_channel << " | Client: " << this->_client;
    if (!this->_reason.empty())
        std::cout << " | Reason: " << this->_reason;
    std::cout << std::endl;
}