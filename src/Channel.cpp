#include "Includes.hpp"

Channel::Channel() : _inviteOnly(false), _topicProtected(false), _userLimit(-1) {}

Channel::Channel(std::string name) : _name(name), _password(""), _inviteOnly(false), _topicProtected(false), _userLimit(-1) {}

// TODO copy

void Channel::addClient(Client &client) {
	this->_Clients[client.getSocketFD()] = &client;
	std::cout << "Client added with FD: " << client.getSocketFD() << std::endl; //DEPOIS TIRAR

}

Client* Channel::getClientById(int socketFD) const {
	std::map<int, Client*>::const_iterator it = _Clients.find(socketFD);
	if (it != _Clients.end()) {
		return it->second;
	}
	return NULL;
}

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
					std::cout << "OLA CARALHO" << std::endl;
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
					Client* clientPtr = getClientById(modeObj.getClientId());
					if (clientPtr) {
						addOperator(modeObj.getClientId(), clientPtr);
						std::cout << "Client with FD " << modeObj.getClientId() << " given operator privileges." << std::endl;
					} else {
						std::cout << "Client with FD " << modeObj.getClientId() << " not found." << std::endl;
					}
				} else {
					removeOperator(modeObj.getClientId());
					std::cout << "Client with FD " << modeObj.getClientId() << " operator privileges removed." << std::endl;
				}
				break;
			default:
				break;
		}
	}
}

void Channel::addOperator(int clientId, Client* client) {
	if (_operators.find(clientId) == _operators.end()) {
		_operators[clientId] = client;
		std::cout << "Operator added: " << clientId << std::endl;
	} else {
		std::cout << "Client " << clientId << " is already an operator." << std::endl;
	}
}

void Channel::removeOperator(int clientId) {
	if (_operators.find(clientId) != _operators.end()) {
		_operators.erase(clientId);
		std::cout << "Operator removed: " << clientId << std::endl;
	} else {
		std::cout << "Client " << clientId << " is not an operator." << std::endl;
	}
}

bool Channel::canJoin(const Client& client) const {
	 if (_inviteOnly) {
		if (!isInvited(_name)) {
			std::cout << "Client " << client.getSocketFD() << " cannot join: not invited." << std::endl;
			return false;
		}
	}
	if (!_password.empty()) {
		if (hasPassword(_password)) { //FIXME - se mando uma senha valida ele entra e nao deveria
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

Channel::~Channel() {}
