#include "Includes.hpp"

void   Client::getClientInfo() {
	struct protoent *proto = getprotobyname("tcp");
	std::cout << "Protocol:" << std::endl;
	std::cout << "\tp_name: " <<  proto->p_name << std::endl;
	std::cout << "\tp_aliases: " << proto->p_aliases << std::endl;
	std::cout << "\tp_proto: " << proto->p_proto << std::endl;

	char hostbuffer[256];
	int hostname;

	hostname = gethostname(hostbuffer, sizeof(hostbuffer));
	if (hostname == -1) {
		throw IRCException("[ERROR] gethostname went wrong");
	}

	std::cout << "Hostname: " << hostbuffer << std::endl;
}

bool Client::isInvited(const std::string& name) const {
	return std::find(_invitedNames.begin(), _invitedNames.end(), name) != _invitedNames.end();
}

void Client::addInvite(const std::string& name) {
	_invitedNames.push_back(name);
}

 bool Client::hasPassword(const std::string& password) const {
	return _enteredPassword == password;
}

void Client::setEnteredPassword(const std::string& password) {
	_enteredPassword = password;
}