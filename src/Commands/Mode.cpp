#include "Includes.hpp"

Mode::Mode(Server& server, Client& client): ACommand("MODE", server, client) {};

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

	if (delimiter == 'k')
		input >> _password;
	else if (delimiter == 'l')
		input >> _userLimit;
	else if (delimiter == 'o')
		input >> _clientId;
}

bool    Mode::isValidMode(char mode) {
	const char validModes[] = {'i', 't', 'k', 'o', 'l'};
	size_t numValidModes = sizeof(validModes) / sizeof(validModes[0]);
	for (size_t i = 0; i < numValidModes; ++i) {
		if (validModes[i] == mode)
			return true;
	}
	return false;
}

void Mode::execute() {
	std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << this->_name << std::endl;
	std::string msg;
	
    std::string channel = this->getChannel();
    std::string modeStr = this->getMode();
    if (channel.empty()) {
        msg.append("Error(461): MODE Not enough parameters.\r\n");
        send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
        return ;
    }
   std::map<std::string, Channel*>channels = this->_server.getChannels();
	std::map<std::string, Channel*>::iterator it = channels.find(this->_channel);
	if (it == channels.end()) {
		std::string msg = "Reply(403): No such channel " + this->_channel + "\r\n";
		send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
		return ;
	}
    Channel* channelObj = it->second;
    for (size_t i = 0; i < modeStr.length(); ++i) {
        char modeChar = modeStr[i];

        if (modeChar != '+' && modeChar != '-' && !isValidMode(modeChar)) {
            msg.append("Error(472): " + std::string(1, modeChar) + " is not a recognised channel mode.");
            send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
            return ;
        }
    }
    channelObj->applyMode(*this); 
}

void Mode::print() const{
	std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << " | Channel: " << this->_channel << std::endl;
}
