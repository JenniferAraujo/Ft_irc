#include "Includes.hpp"

Join::Join(Server& server, Client& client): ACommand("JOIN", server, client) {};

void Join::parsingToken(std::string token, int n) {
	if (n == 0) {
        if (token[0] == '#')
            this->_channels.push(token);	 //JOIN #a //JOIN #a,#b
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
        this->trimChar(token, '\r');
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
                this->trimChar(token, '\r');
				this->parsingToken(token, n);
            }
        }
		n++;
    }
	if (this->_channels.empty())
		this->_error = NEEDMOREPARAMS;
}

void Join::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << this->_name << std::endl;
    std::string msg;
    while(!this->_channels.empty()) {
        std::cout << "Channel: " << this->_channels.front() << std::endl;
		if (this->_password.empty())
		        this->_server.addInChannel(this->_channels.front(),  "", const_cast<Client&>(this->_client));
		else
	        this->_server.addInChannel(this->_channels.front(), this->_password.front(), const_cast<Client&>(this->_client));
        msg.append(JOIN_CHANNEL(this->_client.getNick(), this->_client.getUsername(), this->_client.getIpaddr(), this->_channels.front()));
        send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
        if (this->_server.getChannels().find(this->_channels.front()) != this->_server.getChannels().end()) {
            Channel* channel = this->_server.getChannels()[this->_channels.front()];
            if (channel->getTopic().empty())
                msg.append(RPL_NOTOPIC(this->_server.getHostname(), this->_channels.front(), this->_client.getNick()));
            else
                msg.append(RPL_TOPIC(this->_server.getHostname(), this->_channels.front(), this->_client.getNick(), channel->getTopic()));
            send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
        }
        this->_channels.pop();
		if (!this->_password.empty()) 
        	this->_password.pop();
    }
}

void Join::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << std::endl;
    std::cout << "Channels queue\t| Passwords queue" << std::endl;
	showdoublestringq(this->_channels, this->_password);
}
