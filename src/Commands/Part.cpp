#include "Includes.hpp"

Part::Part(Server& server, Client& client): ACommand("PART", server, client) {};

void Part::parsingToken(std::string token) {
    if (token[0] == '\0' || token[0] == '#')
        this->_channels.push(token);
    else
        this->_error = BADCHANNELKEY;
}

void Part::parsing(std::istringstream &input) {
	std::string token;
    int n = 0;
    while (std::getline(input, token, ' ') || n < 1 ) {
        std::cout << "token: " << token << std::endl;
        this->trimChar(token, '\r');
        int count = std::count(token.begin(), token.end(), ',');
        if (count == 0)
			this->parsingToken(token);
        else {
            std::istringstream tokenstream(token);
            while (std::getline(tokenstream, token, ',')) {
                this->trimChar(token, '\r');
				this->parsingToken(token);
            }
        }
        n++;
    // if (this->_channel.empty() || this->_client.empty())
	// 	this->_error = NEEDMOREPARAMS;
    }
}

void Part::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << this->_name << std::endl;
}

void Part::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << " | Channels: ";
    showstringq(this->_channels);
}