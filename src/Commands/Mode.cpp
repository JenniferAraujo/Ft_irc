#include "Includes.hpp"

Mode::Mode(Server& server, Client& client): ACommand("MODE", server, client) {};

void Mode::parsing(std::istringstream &input){
	std::string mode;
	std::string channel;
	std::string parameters;

	std::getline(input, channel, ' ');
	this->_channel = channel;

	std::getline(input, mode, ' ');
	trimChar(mode, ' ');
	this->_mode = mode;
	
	std::getline(input, parameters);
	trimChar(mode, '\r');
	this->_parameters = parameters;
	extractKeyAndLimit();
	this->print();
	std::cout << "Password: " << this->_password <<  " | User limit: " << this->_userLimit << " | Client ID: " << this->_clientId << std::endl;
}

void	Mode::extractKeyAndLimit() {
	char modeChar = _mode[1];
	std::string parameters = _parameters;

	//TODO - fazer um loop para quando for mais de um mode
	if (modeChar == 'k') {
		trimChar(parameters, '\r');
		this->_password = parameters;
	}// ERR_INVALIDMODEPARAM se for invalid a senha (com espaços)
	else if (modeChar == 'l') {
		bool isValid = true;
		for (size_t i = 0; i < parameters.length(); ++i) {
			if (!isdigit(parameters[i])) {
				isValid = false;
				break;
			}
		}
		if (isValid)
			this->_userLimit = atoi(parameters.c_str());
		else {
			std::cout << "Error: Invalid user limit parameter." << std::endl;
			return ;
		}
	} else if (modeChar == 'o') {
		bool isValid = true;
		for (size_t i = 0; i < parameters.length(); ++i) {
			if (!isdigit(parameters[i])) {
				isValid = false;
				break;
			}
		}
		if (isValid)
			this->_clientId = atoi(parameters.c_str());
		else
			std::cout << "Error: Invalid client ID parameter." << std::endl;
	} else
		std::cout << "Error: Unrecognized mode character: " << modeChar << std::endl;
}

bool Mode::isValidMode(char mode) {
	const char validModes[] = {'i', 't', 'k', 'o', 'l'};
	size_t numValidModes = sizeof(validModes) / sizeof(validModes[0]);
	for (size_t i = 0; i < numValidModes; ++i) {
		if (validModes[i] == mode)
			return true;
	}
	return false;
}

void Mode::execute() {
	std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << this->_name << std::endl;
	std::string msg;
	std::string channel = this->getChannel(); 
	std::string modeStr = this->getMode();
	
	if (channel.empty()) {
		Message::sendMessage(this->_client.getSocketFD(), "Error(461): MODE Not enough parameters.\r\n", this->_server);
		return ;
	}
	std::map<std::string, Channel*> channels = this->_server.getChannels();
	std::map<std::string, Channel*>::iterator it = channels.find(this->_channel);
	if (it == channels.end()) {
		Message::sendMessage(this->_client.getSocketFD(), "Reply(403): No such channel " + this->_channel + "\r\n", this->_server);
		return ;
	}
	Channel* channelObj = it->second;
	if (channelObj == NULL) {
		std::cout << "Error: channelObj is null." << std::endl;
		return ;
	}
	for (size_t i = 0; i < modeStr.length(); ++i) {
		char modeChar = modeStr[i];
		
		std::cout << "modo: " << modeChar << std::endl;
		if (modeChar != '+' && modeChar != '-' && !isValidMode(modeChar)) {
			Message::sendMessage(this->_client.getSocketFD(), "Error(472): " + std::string(1, modeChar) + " is not a recognised channel mode.\r\n", this->_server);
			return ;
		}
		
		if ((modeChar == 'k' || modeChar == 'l' || modeChar == 'o') && i + 1 < modeStr.length() && modeStr[i + 1] == '+') {
			if (modeChar == 'k' && _password.empty()) {
				Message::sendMessage(this->_client.getSocketFD(), "Error(461): MODE +k requires a password.\r\n", this->_server);
				return ;
			}
			if (modeChar == 'l' && _userLimit < 0) {
				Message::sendMessage(this->_client.getSocketFD(), "Error(461): MODE +l requires a user limit.\r\n", this->_server);
				return ;
			}
			if (modeChar == 'o' && channelObj->getClientById(_clientId) == NULL) {
				Message::sendMessage(this->_client.getSocketFD(), "Error(401): No such nick/channel.\r\n", this->_server);
				return ;
			}
		}
	}
	channelObj->applyMode(*this);
	Message::sendMessage(this->_client.getSocketFD(), ":" + this->_client.getNick() + " MODE " + this->_channel + " " + this->_mode + "\r\n", this->_server);
}

void Mode::print() const{
	std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << " | Channel: " << this->_channel << " | Parameters: " << this->_parameters << " | Mode: " << this->_mode << std::endl;
}
