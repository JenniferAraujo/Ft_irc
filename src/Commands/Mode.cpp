#include "Includes.hpp"

Mode::Mode(Server& server, Client& client) : ACommand("MODE", server, client){}

//TODO - COPY

template <typename T>
void	showQueue(const std::queue<T>& q) {
	std::queue<T> tempQueue = q;
	if (tempQueue.empty()) {
		std::cout << "";
	} else {
		while (!tempQueue.empty()) {
			std::cout << tempQueue.front() << " ";
			tempQueue.pop();
		}
	}
	std::cout << "\n";
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

void Mode::extractParameters() {
	std::string aux = _parameters;
	std::string::size_type spacePos;
	char oldModeChar;
	for (std::string::size_type i = 0; i < _mode.length(); ++i) {
		char modeChar = _mode[i];
		if (modeChar == '+' || modeChar == '-') {
			oldModeChar = modeChar;
			continue ;
		}
		switch (modeChar) {
			case 'k':
				spacePos = aux.find(' ');
				if (spacePos != std::string::npos) {
					this->_password.push(aux.substr(0, spacePos));
					aux.erase(0, spacePos + 1);
				} else {
					this->_password.push(aux);
					if (oldModeChar == '+')
						aux.erase();
				}
				break ;
			case 'l':
				spacePos = aux.find(' ');
				if (spacePos != std::string::npos) {
					std::string limitStr = aux.substr(0, spacePos);
					if (isInt(limitStr)) {
						std::stringstream ss(limitStr);						
						int limit;
						ss >> limit;
						this->_userLimit.push(limit);
					}
					aux.erase(0, spacePos + 1);
				} else {
					std::string limitStr = aux;
					if (isInt(limitStr)) {
						std::stringstream ss(limitStr);						
						int limit;
						ss >> limit;
						this->_userLimit.push(limit);
						if (oldModeChar == '+')
							aux.erase();
					}
				}
				break ;
			case 'o':
				spacePos = aux.find(' ');
				if (spacePos != std::string::npos) {
					this->_clientNick.push(aux.substr(0, spacePos));
					aux.erase(0, spacePos + 1);
				} else {
					this->_clientNick.push(aux);
					aux.erase();
				}
				break ;
		}
	}
}

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
				if (!this->existentClientOnChannel(this->_client.getNick(), this->_channel)) {
					this->_error = NOTONCHANNEL;
					return;
				}
				break ;
			case 1: // MODE #a +i || MODE #a i || MODE #a -i
			if (isValidMode(token[0]) || token[0] == '+' || token[0] == '-') {
					this->_mode += token[0];
					token.erase(token.begin());
					for (size_t i = 0; i < token.size(); ++i) {
						char modeChar = token[i];
						if (isValidMode(modeChar) || modeChar == '+' || modeChar == '-') {
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
					if ((token[0] == '+' || token[0] == '-') && isValidMode(token[1]))
						this->_mode += token;
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
	if (this->_channel.empty())
		this->_error = NEEDMOREPARAMS;
	else {
		Channel* channelObj = this->_server.getChannelLower(this->_channel);
		if (!channelObj->isOperator(this->_client.getSocketFD()) && !this->_mode.empty())
			this->_error = CHANOPRIVSNEEDED;
	}	
}

std::string Mode::validParameter(Channel *channel){
	std::string plus = "+";
	std::string minus = "-";
	for (int i = 0; i < (int)this->_mode.length(); ++i){
		if (this->_mode[i] == '+' || isValidMode(this->_mode[i])){
			if (this->_mode[i] == '+')
				i++;
			while(this->_mode[i] != '+' && this->_mode[i] != '-' && this->_mode[i] != '\0'){
				if (this->_mode[i] == 'l') {
					if(this->_userLimit.empty()) //|| this->_userLimit.front().empty())
						Message::sendMessage(this->_client.getSocketFD(), ERR_INVALIDMODEPARAM(this->_server.getHostname(), this->_client.getNick(), this->_channel, "+l", "You must specify a valid parameter for the limit mode"), this->_server);
					else if (this->_userLimit.front() != channel->getUserLimit() && this->_userLimit.front() > 0) {
						plus += this->_mode[i];
						channel->applyMode(*this, _mode[i], true);
						this->_msgUserLimit.push(this->_userLimit.front());
					}
 					else if (this->_userLimit.front() <= 0)
						Message::sendMessage(this->_client.getSocketFD(), ERR_INVALIDMODEPARAM(this->_server.getHostname(), this->_client.getNick(), this->_channel, "+l", "You must specify a valid parameter for the limit mode"), this->_server);
					if(!this->_userLimit.empty())
						this->_userLimit.pop();
				}
				else if (this->_mode[i] == 'o') {
					if(this->_clientNick.empty() || this->_clientNick.front().empty())
						Message::sendMessage(this->_client.getSocketFD(), ERR_INVALIDMODEPARAM(this->_server.getHostname(), this->_client.getNick(), this->_channel, "+o", "You must specify a parameter for the operator mode" ), this->_server);
					else if (channel->getOperatorByNick(_clientNick.front()) == NULL && channel->getClientByNick(_clientNick.front()) != NULL) {
						plus += this->_mode[i];
						channel->applyMode(*this, _mode[i], true);
						this->_msgclientNick.push(this->_clientNick.front());
					} 
					else if (channel->getClientByNick(_clientNick.front()) == NULL && channel->getOperatorByNick(_clientNick.front()) == NULL)
						Message::sendMessage(this->_client.getSocketFD(), ERR_INVALIDMODEPARAM(this->_server.getHostname(), this->_client.getNick(), this->_channel, "+o", "No such nick" ), this->_server);
					if(!this->_clientNick.empty())
						this->_clientNick.pop();
				}
				else if (this->_mode[i] == 'k'){
					if(this->_password.empty() || this->_password.front().empty())
						Message::sendMessage(this->_client.getSocketFD(), ERR_INVALIDMODEPARAM(this->_server.getHostname(), this->_client.getNick(), this->_channel, "+k", "You must specify a parameter for the key mode"), this->_server);
					else if (this->_password.front() != channel->getPassword()) {
						plus += this->_mode[i];
						channel->applyMode(*this, _mode[i], true);
						this->_msgPassword.push(this->_password.front());
					}
					if(!this->_password.empty())
						this->_password.pop();
				}
				else if (this->_mode[i] == 'i') {
					if (!channel->getInviteOnly()) {
						plus += this->_mode[i];
						channel->applyMode(*this, _mode[i], true);
					}
				}
				else if (this->_mode[i] == 't') {
					if (!channel->getTopicProtected()) {
						plus += this->_mode[i];
						channel->applyMode(*this, _mode[i], true);
					}
				}
				i++;
			}
		}
		if (this->_mode[i] == '-'){
			i++;
			while (this->_mode[i] != '+' && this->_mode[i] != '-' && this->_mode[i] != '\0'){
				if (this->_mode[i] == 'l' && channel->getUserLimit() > 0){
					minus += this->_mode[i];
					channel->applyMode(*this, _mode[i], false);
				}
				else if (this->_mode[i] == 'o') {
					if (this->_clientNick.empty() || this->_clientNick.front().empty())
						Message::sendMessage(this->_client.getSocketFD(), ERR_INVALIDMODEPARAM(this->_server.getHostname(), this->_client.getNick(), this->_channel, "-o", "You must specify a parameter for the operator mode" ), this->_server);
					else if (channel->getOperatorByNick(_clientNick.front())) {
						minus += this->_mode[i];
						channel->applyMode(*this, _mode[i], false);
						this->_msgclientNick.push(this->_clientNick.front());
					}
					else if (!channel->getClientByNick(_clientNick.front()))
						Message::sendMessage(this->_client.getSocketFD(), ERR_INVALIDMODEPARAM(this->_server.getHostname(), this->_client.getNick(), this->_channel, "-o", "No such nick" ), this->_server);
					
					else {
						Message::sendMessage(this->_client.getSocketFD(), ERR_INVALIDMODEPARAM(this->_server.getHostname(), this->_client.getNick(), this->_channel, "-o", _clientNick.front() + " is not a operator"), this->_server);
					}
					if (!this->_clientNick.empty())
						this->_clientNick.pop();
				} else if (this->_mode[i] == 'k' && !channel->getPassword().empty()) {
					if(this->_password.empty() || this->_password.front().empty())
						Message::sendMessage(this->_client.getSocketFD(), ERR_INVALIDMODEPARAM(this->_server.getHostname(), this->_client.getNick(), this->_channel, "+k", "You must specify a parameter for the key mode"), this->_server);
					else if (this->_password.front() == channel->getPassword()) {
						minus += this->_mode[i];
						channel->applyMode(*this, _mode[i], false);
						this->_msgPassword.push(this->_password.front());
					}
					if(!this->_password.empty())
						this->_password.pop();
				} else if (this->_mode[i] == 'i' && channel->getInviteOnly()) {
					minus += this->_mode[i];
					channel->applyMode(*this, _mode[i], false);
				} else if (this->_mode[i] == 't' && channel->getTopicProtected()) {
					minus += this->_mode[i];
					channel->applyMode(*this, _mode[i], false);
				}
				i++;
			}
		}
	}
	if (plus.length() == 1 && minus.length() == 1)
		return ("");
	if(plus.length() == 1)
		return(minus);
	if(minus.length() == 1)
		return(plus);
	return plus.append(minus);
}

void Mode::execute() {
	std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << this->_name << std::endl;
	switch (this->_error) {
		case NOSUCHCHANNEL:
			Message::sendMessage(this->_client.getSocketFD(), ERR_NOSUCHCHANNEL(this->_server.getHostname(), this->_client.getNick(), this->_channel), this->_server);
			break ;
		case UNKNOWNMODE:
			Message::sendMessage(this->_client.getSocketFD(), ERR_UNKNOWNMODE(this->_server.getHostname(), this->_client.getNick(), this->_modeChar), this->_server);
			break ;
		case NEEDMOREPARAMS:
			Message::sendMessage(this->_client.getSocketFD(), ERR_NEEDMOREPARAMS(this->_server.getHostname(), this->_client.getNick(), this->_name), this->_server);
			break ;
		case CHANOPRIVSNEEDED:
			Message::sendMessage(this->_client.getSocketFD(), ERR_CHANOPRIVSNEEDED(this->_server.getHostname(), this->_client.getNick(), this->_channel), this->_server);
			break ;
		 default: {
			Channel* channelObj = this->_server.getChannelLower(this->_channel);
			if (!this->_mode.empty()) {
				std::string msg = validParameter(channelObj);
				if (!msg.empty())
					channelObj->sendMessage(RPL_MODE(this->_client.getNick(), this->_client.getUsername(), this->_client.getIpaddr(), this->_channel, msg, queueIntToString(_msgUserLimit), queueStrToString(_msgPassword), queueStrToString(_msgclientNick)), 0);
			} else {
				std::string msg;
				if (!channelObj->getPassword().empty())
					msg.append("k");
				if (channelObj->isTopicLocked())
					msg.append("t");
				if (channelObj->getInviteOnly())
					msg.append("i");
				if (channelObj->getUserLimit() != -1)
					msg.append("l");
				Message::sendMessage(this->_client.getSocketFD(),RPL_ONLYMODE(this->_client.getNick(), this->_server.getHostname(), this->_channel, msg, intToString(channelObj->getUserLimit()), channelObj->getPassword()), this->_server);
			}
			this->_server.printChannelInfo(this->_channel);
			this->print();
		}
	}
}

std::string Mode::queueIntToString(std::queue<int> q) {
	std::ostringstream oss;
	while (!q.empty()) {
		oss << q.front();
		q.pop();
		if (!q.empty()) {
			oss << " ";
		}
	}
	return oss.str();
}

std::string	Mode::queueStrToString(std::queue<std::string> q) {
	std::ostringstream oss;
	while (!q.empty()) {
		oss << q.front();
		q.pop();
		if (!q.empty()) {
			oss << " ";
		}
	}
	return oss.str();
}

void Mode::print() const{	
	if (this->_error != 0)
		std::cout << " " << RED << "[" << this->_error << "]" << std::endl;
}

