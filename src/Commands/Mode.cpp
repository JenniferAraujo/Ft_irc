#include "Includes.hpp"

Mode::Mode(Server& server, Client& client): ACommand("MODE", server, client) { this->_userLimit = -1;};

//FIXME - Trabalhar no parsing
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
	extractParameters();
	this->print();
}

void Mode::extractParameters() {
	std::string aux = _parameters;
	std::string::size_type spacePos;
	for (std::string::size_type i = 0; i < _mode.length(); ++i) {
		char modeChar = _mode[i];
		if (modeChar == '+' || modeChar == '-')
			continue;
		switch (modeChar) {
			case 'k':
				spacePos = aux.find(' ');
				if (spacePos != std::string::npos) {
					this->_password = aux.substr(0, spacePos);
					aux.erase(0, spacePos + 1);
				} else
					this->_password = aux;
				break;
			case 'l':
				spacePos = aux.find(' ');
				if (spacePos != std::string::npos) {
					std::string limitStr = aux.substr(0, spacePos);
					bool isValid = true;
					for (std::string::size_type j = 0; j < limitStr.length(); ++j) {
						if (!isdigit(limitStr[j])) {
							isValid = false;
							break;
						}
					}
					if (isValid) {
						std::stringstream ss(limitStr);
						ss >> this->_userLimit;
					}
					aux.erase(0, spacePos + 1);
				} else {
					std::string limitStr = aux;
					bool isValid = true;
					for (std::string::size_type j = 0; j < limitStr.length(); ++j) {
						if (!isdigit(limitStr[j])) {
							isValid = false;
							break;
						}
					}
					if (isValid) {
						std::stringstream ss(limitStr);
						ss >> this->_userLimit;
					}
				}
				break;
			case 'o':
				spacePos = aux.find(' ');
				if (spacePos != std::string::npos) {
					this->_clientNick = aux.substr(0, spacePos);
					aux.erase(0, spacePos + 1);
				} else
					this->_clientNick = aux;
				break;
		}
	}
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
	if (_channel.empty()) {
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
	for (size_t i = 0; i < _mode.length(); ++i) {
		char modeChar = _mode[i];
		if (modeChar != '+' && modeChar != '-' && !isValidMode(modeChar)) {
			Message::sendMessage(this->_client.getSocketFD(), "Error(472): " + std::string(1, modeChar) + " is not a recognised channel mode.\r\n", this->_server);
			return ;
		}
		if ((modeChar == 'k' || modeChar == 'l' || modeChar == 'o') && i + 1 < _mode.length() && _mode[i + 1] == '+') {
			if (modeChar == 'k' && _password.empty()) {
				Message::sendMessage(this->_client.getSocketFD(), "Error(461): MODE +k requires a password.\r\n", this->_server);
				return ;
			}
			if (modeChar == 'l' && _userLimit < 0) {
				Message::sendMessage(this->_client.getSocketFD(), "Error(461): MODE +l requires a user limit.\r\n", this->_server);
				return ;
			}
			if (modeChar == 'o' && channelObj->getClientByNick(_clientNick) == NULL) {
				Message::sendMessage(this->_client.getSocketFD(), "Error(401): No such nick/channel.\r\n", this->_server);
				return ;
			}
		}
	}
	channelObj->applyMode(*this);
    this->_server.printChannelInfo(this->_channel);
	Message::sendMessage(this->_client.getSocketFD(), ":" + this->_client.getNick() + " MODE " + this->_channel + " " + this->_mode + "\r\n", this->_server);
}

void Mode::print() const{
	std::cout << "User limit: " << this->_userLimit <<  " | Nick: " << this->_clientNick << " | Password: " << this->_password << " | Parameters: " << this->_parameters << " | Mode: " << this->_mode << std::endl;
}
