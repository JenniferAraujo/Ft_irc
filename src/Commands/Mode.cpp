#include "Includes.hpp"

Mode::Mode(const Server& server, const Client& client): ACommand("MODE", server, client) {};

void Mode::parsing(std::istringstream &input){
	std::string mode;
	std::string channel;

	std::getline(input, channel, ' ');
	channel.erase(0, 1);
	this->_channel = channel;

	std::getline(input, mode);
	this->trimChar(mode, '\r');
	this->_mode = mode;
	extractKeyAndLimit(input);
}

void	Mode::extractKeyAndLimit(std::istringstream &input) {
	char delimiter;
	input >> delimiter;

	if (delimiter == 'k') {
		input >> _password;
	} else if (delimiter == 'l') {
		input >> _userLimit;
	} else if (delimiter == 'o') {
		input >> _clientId;
	}
}

