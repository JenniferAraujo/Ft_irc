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
	if (this->_channels.empty())
		this->_error = NEEDMOREPARAMS;
}

//TODO - Tratamento de Erros
//TODO - Lógica de remover do invited caso tenha entrado (ou confirmar se o invite é efetivamente removido ou permanente)
void Join::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << this->_name;
    this->print();
    switch (this->_error) {
        case NEEDMOREPARAMS:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NEEDMOREPARAMS(this->_server.getHostname(), this->_client.getNick(), this->_name), this->_server);
            break;
        default:
            while(!this->_channels.empty()) {
                this->_server.addInChannel(this->_channels.front(),(this->_password.empty() ? "" : this->_password.front()), const_cast<Client&>(this->_client));
                Channel* ch = this->_server.getChannels()[this->_channels.front()];
                ch->sendMessage(JOIN_CHANNEL(this->_client.getNick(), this->_client.getUsername(), this->_client.getIpaddr(), this->_channels.front()), 0);
                this->_client.setJustJoined(true);
                if (ch->getTopic().empty())
                    Message::sendMessage(this->_client.getSocketFD(), RPL_NOTOPIC(this->_server.getHostname(), this->_channels.front(), this->_client.getNick()), this->_server);
                else
                    Message::sendMessage(this->_client.getSocketFD(), RPL_TOPIC(this->_server.getHostname(), this->_channels.front(), this->_client.getNick(), ch->getTopic()), this->_server);
                this->_channels.pop();
                if (!this->_password.empty())
                    this->_password.pop();
            }
            break;
    }
}

void Join::print() const{
    //std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << std::endl;
    //std::cout << "Channels queue\t| Passwords queue" << std::endl;
	//showdoublestringq(this->_channels, this->_password);
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
