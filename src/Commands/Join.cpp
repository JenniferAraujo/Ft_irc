#include "Includes.hpp"

Join::Join(Server& server, Client& client): ACommand("JOIN", server, client) {};

void Join::parsingToken(std::string token, int n) {
	if (n == 0) {
		if (token[0] == '#' || token.length() <= MAXCHARS) {
			if (!findInQueue(this->_channels, token))
				this->_channels.push(token);	 //JOIN #a //JOIN #a,#b
		}
		else
			this->_error = BADCHANNELKEY;	 //JOIN a //JOIN a,#b //JOIN #a,b
	} else {
		if (token[0] == '\0' || token[0] == '#')
			return;						 	 //JOIN #a, #b //JOIN #a ,#b
		else
			this->_password.push(token);	 //JOIN #a 123 //JOIN #a,#b 123,312
	}
}

void Join::parsing(std::istringstream &input){
	std::string token;
	int n = 0;
	while (std::getline(input, token, ' ') && n < 2) {
		trimChar(token, '\r');
		if (token.empty()) {
			this->_error = NEEDMOREPARAMS;  //JOIN  #a
			return;
		}
		int count = std::count(token.begin(), token.end(), ',');
		if (count == 0)
			this->parsingToken(token, n);
		else {
			std::istringstream tokenstream(token);
			while (std::getline(tokenstream, token, ',')) {
				trimChar(token, '\r');
				this->parsingToken(token, n);
			}
		}
		n++;
	}
	if (this->_error == 0) {
		if (this->_channels.empty())
			this->_error = NEEDMOREPARAMS;
		else if(this->_channels.size() > MAX_TARGETS)
        	this->_error = TOOMANYTARGETS;
	}
}

void Join::execute() {
	std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << this->_name;
	this->print();
	switch (this->_error) {
		case NEEDMOREPARAMS:
			Message::sendMessage(this->_client.getSocketFD(), ERR_NEEDMOREPARAMS(this->_server.getHostname(), this->_client.getNick(), this->_name), this->_server);
			break;
		default:
			while(!this->_channels.empty()) {
				int	aux = 0;
				if ((aux = this->_server.addInChannel(this->_channels.front(),(this->_password.empty() ? "" : this->_password.front()), const_cast<Client&>(this->_client))) == 0) {
					Channel* ch = this->_server.getChannelLower(this->_channels.front());
					ch->sendMessage(JOIN_CHANNEL(this->_client.getNick(), this->_client.getUsername(), this->_client.getIpaddr(), ch->getName()), 0);
					this->_client.setJustJoined(true, ch->getName());
					if (ch->getTopic().empty())
						Message::sendMessage(this->_client.getSocketFD(), RPL_NOTOPIC(this->_server.getHostname(), ch->getName(), this->_client.getNick()), this->_server);
					else
						Message::sendMessage(this->_client.getSocketFD(), RPL_TOPIC(this->_server.getHostname(), ch->getName(), this->_client.getNick(), ch->getTopic()), this->_server);
					this->_channels.pop();
					if (!this->_password.empty())
						this->_password.pop();
				} else {
					switch (aux) {
    					case CHANNELISFULL:
            				Message::sendMessage(this->_client.getSocketFD(), ERR_CHANNELISFULL(this->_server.getHostname(), this->_client.getNick(), this->_channels.front()), this->_server);
            			break;
        				case INVITEONLYCHAN:
							Message::sendMessage(this->_client.getSocketFD(), ERR_INVITEONLYCHAN(this->_server.getHostname(), this->_client.getNick(), this->_channels.front()), this->_server);
							break;
						case USERONCHANNEL:
							Message::sendMessage(this->_client.getSocketFD(), ERR_USERONCHANNEL(this->_server.getHostname(), this->_client.getNick(), this->_client.getNick(),this->_channels.front()), this->_server);
							break;
						case BADCHANNELKEY:
							Message::sendMessage(this->_client.getSocketFD(), ERR_BADCHANNELKEY(this->_server.getHostname(), this->_client.getNick(), this->_channels.front()), this->_server);
							break; }
					return ;
				}
			}
			break;
	}
}

void Join::print() const{
	if (this->_error != 0)
		std::cout << " " << RED << "[" << this->_error << "]" << std::endl;
	else {
		std::cout << "\t[";
		showstringq(this->_channels);
		std::cout << "] [";
		showstringq(this->_password);
		std::cout << "]" << std::endl;
	}
}
