#include "Includes.hpp"

Part::Part(Server& server, Client& client): ACommand("PART", server, client) {};

void Part::parsingToken(std::string token, int n) {
    if (n == 0) {
        if (token[0] != '#' || token.empty())
            this->_error = BADCHANNELKEY;   //PART a,#b //PART a,#b
        else
            this->_channels.push(token);    //PART #a //PART #a,#b
    }
    else {
        if (token[0] == ':') {
            token.erase(token.begin());
            this->_message = token;         //PART #a :Bye! //PART #a,#b :Bye!
        }
        else
            this->_error = BADCHANNELKEY;   //PART #a Bye!
    }
}

void Part::parsing(std::istringstream &input) {
	std::string token;
    int n = 0;
    while (std::getline(input, token, ' ') || n < 1 ) {
        trimChar(token, '\r');
        if (token.empty()) {
            this->_error = NEEDMOREPARAMS;  //PART  #a
            return;
        }
        int count = std::count(token.begin(), token.end(), ',');
        if (count == 0)
			this->parsingToken(token, n);
        else {
            std::istringstream tokenstream(token);
            while (std::getline(tokenstream, token, ',')) {
                trimChar(token, '\r');
				this->parsingToken(token, n);
            }
        }
        n++;
    }
    if (this->_channels.empty())
		this->_error = NEEDMOREPARAMS;
}

void Part::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << this->_name << std::endl;
}

void Part::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << std::endl;
    std::cout << "Channels queue:" << std::endl;
	showstringq(this->_channels);
    std::cout << "Message: " << this->_message << std::endl;
}