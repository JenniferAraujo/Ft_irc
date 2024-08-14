#include "Includes.hpp"

Channel::Channel(){}

Channel::Channel(std::string name): _name(name){}

//TODO copy

void Channel::addClient(Client &client) {
	this->_Clients[client.getSocketFD()] = &client;
}

Client* Channel::getClientById(int id) const {
    std::map<int, Client*>::const_iterator it = _Clients.find(id);
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
				break;
			case 't':
				_topicProtected = adding;
				break;
			case 'k':
				if (adding) {
					setPassword(modeObj.getPassword());

				} else {
					setPassword("");
				}
				break;
			case 'l':
				if (adding) {
					setUserLimit(modeObj.getLimit());
				} else {
					setUserLimit(-1);
				}
				break;
			case 'o':
				if (adding) {
					Client* clientPtr = getClientById(modeObj.getClientId());
					addOperator(modeObj.getClientId(), clientPtr);
				} else {
					removeOperator(modeObj.getClientId());
				}
				break;
			default:
				//lidar com os casos que nao sao os comandos possiveis
				break;
		}
	}
}

//usar depois a logica dessa funcao p verificar se o usuario pode entrar no canal 
bool Channel::canJoin(const Client& client) const {
	if (isPasswordProtected() && !client.hasPassword(_password)) {
		return true;
	}
	if (isInviteOnly() && !client.isInvited(_name)) {
		return true;
	}
	if (_userLimit > 0 && static_cast<size_t>(_Clients.size()) >= static_cast<size_t>(_userLimit)) {
		return true;
	}
	return false;
}


Channel::~Channel() {}