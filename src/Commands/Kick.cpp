#include "Includes.hpp"

Kick::Kick(Server& server, Client& client): ACommand("KICK", server, client) {};

void Kick::parsing(std::istringstream &input) {
	std::string token;
    int n = 0;
    while (std::getline(input, token, ' ') || n < 3) {
        this->trimChar(token, '\r');
        if (token.empty()) {
            this->_error = NEEDMOREPARAMS;          //KICK  #a Diogo
            return;
        }
        switch (n)
        {
            case 0:
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
                break;
            case 1:
                if (this->existentClient(token))
                    this->_client = token;			//KICK #a Diogo
                else {
                    this->_error = USERNOTINCHANNEL;
                    return ;
                }
                break;
            case 2:
                if (token[0] == ':') {
                    token.erase(token.begin());
                    this->_reason = token;			//KICK #a Diogo :idk
                }
                else {
                    this->_error = NEEDMOREPARAMS;	//KICK #a Diogo idk
                    return ;
                }
                break;
        }
        n++;
    }
    if (this->_channel.empty() || this->_client.empty())
		this->_error = NEEDMOREPARAMS;
}

void Kick::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << this->_name << std::endl;
    //TODO - Permissões de OPERADOR
    /*if (this->_server.getChannels().find(this->_channel) != this->_server.getChannels().end()) {
        Channel* channel = this->_server.getChannels()[this->_channel];
        //if (channel.)
    }*/
}

void Kick::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error;
    std::cout << " | Channel: " << this->_channel << " | Client: " << this->_client;
    if (!this->_reason.empty())
        std::cout << " | Reason: " << this->_reason;
    std::cout << std::endl;
}