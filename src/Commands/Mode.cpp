#include "Includes.hpp"

Mode::Mode(Server& server, Client& client): ACommand("MODE", server, client) { this->_userLimit = -1;};

//TODO - COPY

void Mode::parsing(std::istringstream &input) {
	std::string token;
	int n = 0;
	bool modeFlag = false;
	std::vector<std::string> collectedParameters;

	while (std::getline(input, token, ' ')) {
		trimChar(token, '\r');
		if (token.empty()) {
			this->_error = NEEDMOREPARAMS;
			return ;
		}
		switch (n) {
			case 0:
				this->_channel = token;
				if (token[0] != '#' || !this->existentChannel(token)) {
					this->_error = NOSUCHCHANNEL;
					return ;
				}
				break ;
			case 1: // MODE #a +i || MODE #a i || MODE #a -i
			if (isValidMode(token[0]) || token[0] == '+' || token[0] == '-') {
					this->_mode += token[0];
					token.erase(token.begin());
					for (size_t i = 0; i < token.size(); ++i) {
						char modeChar = token[i];
						if (isValidMode(modeChar)) {
							this->_mode += modeChar;
						} else {
							this->_error = UNKNOWNMODE;
							return ;
						}
					}
					modeFlag = true;
				} else {
					this->_error = UNKNOWNMODE;
					return ;
				}
				break ;
			default:
				if (modeFlag) {
					if (token[0] == '+' || token[0] == '-') {
                    	token.erase(token.begin());
						this->_mode += token;
					}
					else
						collectedParameters.push_back(token);
				}
				break ;
		}
		n++;
	}
	this->_parameters.clear();
	for (size_t i = 0; i < collectedParameters.size(); ++i) {
		if (i != 0)
			this->_parameters += " ";
		this->_parameters += collectedParameters[i];
	}
	extractParameters();
	if (this->_error == 0) {
		bool adding = true;
		for (size_t i = 0; i < _mode.length(); ++i) {
			char modeChar = _mode[i];
			this->_modeChar = modeChar;
			if (modeChar == '-')
				adding = false;
			Channel* channelObj = this->_server.getChannels()[this->_channel];
			if (adding) {
				if (modeChar == 'o' && (channelObj->getClientByNick(_clientNick) == NULL)) {
					this->_error = INVALIDMODEPARAM;
					return ;
				}
				if (modeChar == 'l' && _userLimit < 0) {
					this->_error = INVALIDMODEPARAM;
					return ;
				}
				if (modeChar == 'k' && _password.empty()) {
					this->_error = INVALIDMODEPARAM;
					return ;
				}
			} else {
				if (modeChar == 'o' && (channelObj->getOperatorByNick(_clientNick) == NULL)) {
					this->_error = INVALIDMODEPARAM;
					return ;
				}
			}
		}
		if (this->_channel.empty())
			this->_error = NEEDMOREPARAMS;
		else {
			Channel* ch = this->_server.getChannels()[this->_channel];
			if (!ch->isOperator(this->_client.getSocketFD()))
				this->_error = CHANOPRIVSNEEDED;
		}
	}
}

void Mode::extractParameters() {
	std::string aux = _parameters;
	std::string::size_type spacePos;
	this->print();
	for (std::string::size_type i = 0; i < _mode.length(); ++i) {
		char modeChar = _mode[i];
		if (modeChar == '+' || modeChar == '-')
			continue ;
		switch (modeChar) {
			case 'k':
				spacePos = aux.find(' ');
				if (spacePos != std::string::npos) {
					this->_password = aux.substr(0, spacePos);
					aux.erase(0, spacePos + 1);
				} else
					this->_password = aux;
				break ;
			case 'l':
				spacePos = aux.find(' ');
				if (spacePos != std::string::npos) {
					std::string limitStr = aux.substr(0, spacePos);
					bool isValid = true;
					for (std::string::size_type j = 0; j < limitStr.length(); ++j) {
						if (!isdigit(limitStr[j])) {
							isValid = false;
							break ;
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
							break ;
						}
					}
					if (isValid) {
						std::stringstream ss(limitStr);
						ss >> this->_userLimit;
					}
				}
				break ;
			case 'o':
				spacePos = aux.find(' ');
				if (spacePos != std::string::npos) {
					this->_clientNick = aux.substr(0, spacePos);
					aux.erase(0, spacePos + 1);
				} else
					this->_clientNick = aux;
				break ;
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
	std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << this->_name;
	this->print();
	switch (this->_error) {
		case NOSUCHCHANNEL:
			Message::sendMessage(this->_client.getSocketFD(), ERR_NOSUCHCHANNEL(this->_server.getHostname(), this->_client.getNick(), this->_channel), this->_server);
			break ;
		case UNKNOWNMODE:
			Message::sendMessage(this->_client.getSocketFD(), ERR_UNKNOWNMODE(this->_server.getHostname(), this->_client.getNick(), this->_modeChar), this->_server);
			break ;
		case INVALIDMODEPARAM:
			Message::sendMessage(this->_client.getSocketFD(), ERR_INVALIDMODEPARAM(this->_server.getHostname(), this->_client.getNick(), this->_channel, this->_modeChar), this->_server);
			break ;
		case NEEDMOREPARAMS:
			Message::sendMessage(this->_client.getSocketFD(), ERR_NEEDMOREPARAMS(this->_server.getHostname(), this->_client.getNick(), this->_name), this->_server);
			break ;
		case CHANOPRIVSNEEDED:
			Message::sendMessage(this->_client.getSocketFD(), ERR_CHANOPRIVSNEEDED(this->_server.getHostname(), this->_client.getNick(), this->_channel), this->_server);
			break ;
		default:
			Channel* channelObj = this->_server.getChannels()[this->_channel];
			channelObj->applyMode(*this);
			this->_server.printChannelInfo(this->_channel);
			if (!this->_mode.empty())
				channelObj->sendMessage(RPL_MODE(this->_client.getNick(), this->_client.getUsername(), this->_client.getIpaddr(), this->_channel, this->_mode, intToString(this->_userLimit), this->_password, this->_clientNick), 0);
			else
				channelObj->sendMessage(RPL_ONLYMODE(this->_client.getNick(), this->_server.getHostname(),this->_channel, channelObj->getMode(), intToString(channelObj->getUserLimit()), channelObj->getPassword()), 0);
	}
}

void Mode::print() const{
	if (this->_error != 0)
		std::cout << " " << RED << "[" << this->_error << "]" << std::endl;
	std::cout << "\nUser limit: " << this->_userLimit <<  " | Nick: " << this->_clientNick << " | Password: " << this->_password << " | Parameters: " << this->_parameters << " | Mode: " << this->_mode << std::endl;
}
