#include "Includes.hpp"

Channel::Channel() : _inviteOnly(false), _topicProtected(false), _userLimit(-1) {}

Channel::Channel(std::string name) : _name(name), _password(""), _inviteOnly(false), _topicProtected(false), _userLimit(-1) {}

// TODO copy

void Channel::addClient(Client &client) {
	this->_Clients[client.getSocketFD()] = &client; }

void Channel::applyMode(const Mode& modeObj) {
	std::string modeStr = modeObj.getMode();
	bool adding = true;

	for (size_t i = 0; i < modeStr.length(); ++i) {
		char modeChar = modeStr[i];

		if (modeChar == '+' || modeChar == '-') {
			adding = (modeChar == '+');
			continue;
		}
		switch (modeChar) {
			case 'i':
				_inviteOnly = adding;
				std::cout << "Invite-Only mode set to: " << (adding ? "ON" : "OFF") << std::endl;
				break;
			case 't':
				_topicProtected = adding;
				std::cout << "Topic-Protection mode set to: " << (adding ? "ON" : "OFF") << std::endl;
				break;
			case 'k':
				if (adding) {
					setPassword(modeObj.getPassword());
					std::cout << "Password set to: " << modeObj.getPassword() << std::endl;
				} else {
					setPassword("");
					std::cout << "Password removed." << std::endl;
				}
				break;
			case 'l':
				if (adding) {
					setUserLimit(modeObj.getLimit());
					std::cout << "User limit set to: " << modeObj.getLimit() << std::endl;
				} else {
					setUserLimit(-1);
					std::cout << "User limit removed." << std::endl;
				}
				break;
			case 'o':
				if (adding) {
					Client* clientPtr = getClientByNick(modeObj.getClientNick());
					if (clientPtr) {
						int clientFd = clientPtr->getSocketFD();
						addOperator(clientFd, clientPtr);
						printOperators();
						std::cout << "Client with Nick " << modeObj.getClientNick() << " given operator privileges." << std::endl;
					} else {
						std::cout << "Client with Nick " << modeObj.getClientNick() << " not found." << std::endl;
					}
				} else {
					Client* clientPtr = getClientByNick(modeObj.getClientNick());
					if (clientPtr) {
						int clientFd = clientPtr->getSocketFD();
						removeOperator(clientFd);
						std::cout << "Client with Nick " << modeObj.getClientNick() << " operator privileges removed." << std::endl;
					} else {
						std::cout << "Client with Nick " << modeObj.getClientNick() << " not found." << std::endl;
					}
				}
				break;
		}
	}
}

void Channel::addOperator(int clientId, Client* client) {
	if (_operators.find(clientId) == _operators.end())
		_operators[clientId] = client;
}

void Channel::removeOperator(int clientId) {
	if (_operators.find(clientId) != _operators.end())
		_operators.erase(clientId);
}

void Channel::removeClient(int clientId) {
	if (_Clients.find(clientId) != _Clients.end())
		_Clients.erase(clientId);
}

void Channel::removeInvited(int clientId) {
	_invitedClients.erase(std::remove(_invitedClients.begin(), _invitedClients.end(), clientId), _invitedClients.end());
}

bool Channel::canJoin(const Client& client, std::string password) const {
	 if (_inviteOnly) {
		if (!isInvited(client.getSocketFD())) {
			std::cout << "Client " << client.getSocketFD() << " cannot join: not invited." << std::endl;
			return false;
		}
	}
	if (!_password.empty()) {
		if (!hasPassword(password)) { 
			std::cout << "Client " << client.getSocketFD() << " cannot join: invalid password" << std::endl;
			//TODO - ERR_BADCHANNELKEY (475) -- Adicionar validações de senha esta correta, se estiver invalida ERR_INVALIDMODEPARAM
			return false;
		}
	}
	if (_userLimit > 0 && static_cast<size_t>(_Clients.size()) >= static_cast<size_t>(_userLimit)) {
		std::cout << "Client " << client.getSocketFD() << " cannot join: channel is full." << std::endl;
		return false;
	}
	std::cout << "Client " << client.getSocketFD() << " can join the channel." << std::endl;
	return true;
}

void	Channel::sendMessageToOperators(std::string msg, int skipFD) {
	std::map<int, Client*> operators = this->getOperators();
    for (std::map<int, Client*>::iterator it = operators.begin(); it != operators.end(); ++it) {
        Client* client = it->second;
        if (client->getSocketFD() != skipFD)
            Message::sendMessage(client->getSocketFD(), msg, client->getServer());
    }
}

void	Channel::sendMessageToClients(std::string msg, int skipFD) {
	std::map<int, Client*> clients = this->getClients();
	for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
		Client* client = it->second;
		if (client->getSocketFD() != skipFD){
			Message::sendMessage(client->getSocketFD(), msg, client->getServer());
		}
	}
}

void	Channel::sendMessage(std::string msg, int skipFD) {
	this->sendMessageToOperators(msg, skipFD);
	this->sendMessageToClients(msg, skipFD);
}

Channel::~Channel() {}
