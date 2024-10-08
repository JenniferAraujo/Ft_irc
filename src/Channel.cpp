#include "Includes.hpp"

Channel::Channel() : _inviteOnly(false), _topicProtected(false), _userLimit() {}

Channel::Channel(std::string name) : _name(name), _password(""), _inviteOnly(false), _topicProtected(false), _userLimit(-1) {}
// TODO copy

void Channel::addClient(Client &client) {
	this->_Clients[client.getSocketFD()] = &client;
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

int	Channel::canJoin(const Client& client, std::string password) const {
	 if (_inviteOnly) {
		if (!isInvited(client.getSocketFD()))
			return INVITEONLYCHAN;
	}
	if (!_password.empty()) {
		if (!hasPassword(password))
			return BADCHANNELKEY;
	}
	if ((_userLimit < 0) || (_Clients.size() + _operators.size()) >= (unsigned long)_userLimit)
		return CHANNELISFULL;
	if (isClient(client.getSocketFD()) || isOperator(client.getSocketFD()))
		return USERONCHANNEL;
	return 0;
}

void Channel::applyMode(const Mode& modeObj, char modeChar, bool adding) {
		switch (modeChar) {
			case 'i':
				_inviteOnly = adding;
				break;
			case 't':
				_topicProtected = adding;
				break;
			case 'k':
				if (adding)
					setPassword(modeObj.getPassword().front());
				else
					setPassword("");
				break;
			case 'l':
				if (adding)
					setUserLimit(modeObj.getLimit().front());
				else
					setUserLimit(-1);
				break;
			case 'o':
				if (adding) {
					Client* clientPtr = getClientByNick(modeObj.getClientNick().front());
					if (clientPtr) {
						int clientFd = clientPtr->getSocketFD();
						addOperator(clientFd, clientPtr);
						removeClient(clientPtr->getSocketFD());
					}
				} else {
					Client* clientPtr = getOperatorByNick(modeObj.getClientNick().front());
					if (clientPtr) {
						removeOperator(clientPtr->getSocketFD());
						addClient(*clientPtr);
					}
				break;
				}
	 	}
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
		if (client->getSocketFD() != skipFD)
			Message::sendMessage(client->getSocketFD(), msg, client->getServer());
	}
}

void	Channel::sendMessage(std::string msg, int skipFD) {
	this->sendMessageToOperators(msg, skipFD);
	this->sendMessageToClients(msg, skipFD);
}

Client* Channel::getClientByNick(const std::string& nick) {
		for (std::map<int, Client*>::const_iterator it = _Clients.begin(); it != _Clients.end(); ++it) {
			Client* client = it->second;
			if (toLowerCase(client->getNick()) == toLowerCase(nick)) {
				return client;
			}
		}
		return NULL;  
}

Client* Channel::getOperatorByNick(const std::string& nick) {
	for (std::map<int, Client*>::const_iterator it = _operators.begin(); it != _operators.end(); ++it) {
		Client* client = it->second;
		if (toLowerCase(client->getNick()) == toLowerCase(nick)) {
			return client;
		}
	}
	return NULL;  
}

void	Channel::printOperators() const { //NOTE - Apagar depois
		std::cout << "Operators in the channel:" << std::endl;
		for (std::map<int, Client*>::const_iterator it = _operators.begin(); it != _operators.end(); ++it) {
			int clientFd = it->first;
			Client* clientPtr = it->second;
			if (clientPtr)
				std::cout << "Operator FD: " << clientFd << ", Nick: " << clientPtr->getNick() << std::endl; 
		} 
};

Channel::~Channel() {}
