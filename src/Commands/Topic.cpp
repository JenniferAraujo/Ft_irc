#include "Includes.hpp"

Topic::Topic(Server& server, Client& client): ACommand("TOPIC", server, client) {};

void Topic::parsing(std::istringstream &input){
	std::string token;
    int n = 0;
    while (std::getline(input, token, ' ') || n < 1) {
        this->trimChar(token, '\r');
        if (token.empty()) {
            this->_error = NEEDMOREPARAMS;          //TOPIC  #a
            return;
        }
        switch (n)
        {
            case 0:
                if (token[0] == '#') {
                    if (this->existentChannel(token))
                        this->_channel = token;     //TOPIC #a
                    else {
                        this->_error = NOSUCHCHANNEL;
                        return ;
                    }
                }
                else {
                    this->_error = BADCHANMASK;     //TOPIC a
                    return ;
                }
                break;
            case 1:
                if (token[0] == ':') {
                    token.erase(token.begin());
                    this->_msg = token;			//TOPIC #a :Ajuda
                }
                else {
                    this->_error = NEEDMOREPARAMS;	//TOPIC #a Ajuda
                    return ;
                }
                break;
        }
    }
}

void Topic::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << this->_name << std::endl;
}

void Topic::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << " | Token: ";
}