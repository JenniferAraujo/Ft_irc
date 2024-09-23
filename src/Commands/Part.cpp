#include "Includes.hpp"

Part::Part(Server& server, Client& client): ACommand("PART", server, client) {};

void Part::parsingToken(std::string token, int n, std::istringstream &input) {
    if (n == 0) {
        this->_channels.push(token);    //PART #a //PART #a,#b
        if (token[0] != '#' || token.empty() || !this->existentChannel(token)) {
            while (!this->_channels.empty()) {
                this->_channels.pop();
            }
            this->_channels.push(token);
            this->_error = NOSUCHCHANNEL;   //PART a,#b //PART a,#b
            return;
        } else if (!this->existentClientOnChannel(this->_client.getNick(), this->_channels.front())) {
            while (!this->_channels.empty()) {
                this->_channels.pop();
            }
            this->_channels.push(token);
            this->_error = NOTONCHANNEL;        //NÃO ESTIVER NO CANAL
            return;
        }
    }
    else {
        if (token[0] == ':') {
            token.erase(token.begin());
            this->_message = token;         //PART #a :Bye! //PART #a,#b :Bye!
            while (std::getline(input, token, ' ')) {
                trimChar(token, '\r');
                this->_message.append(" ").append(token);
            }
        }
    }
}

void Part::parsing(std::istringstream &input) {
	std::string token;
    int n = 0;
    while (std::getline(input, token, ' ') || n < 1 ) {
        trimChar(token, '\r');
        if (token.empty()) {
            this->_error = NEEDMOREPARAMS;  //PART  #a
            return;
        }
        int count = std::count(token.begin(), token.end(), ',');
        if (count == 0)
			this->parsingToken(token, n, input);
        else {
            std::istringstream tokenstream(token);
            while (std::getline(tokenstream, token, ',')) {
                trimChar(token, '\r');
				this->parsingToken(token, n, input);
            }
        }
        n++;
    }
    if (this->_error == 0 && this->_channels.empty())
		this->_error = NEEDMOREPARAMS;
}

void Part::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << this->_name;
    this->print();
    switch (this->_error) {
        case NEEDMOREPARAMS:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NEEDMOREPARAMS(this->_server.getHostname(), this->_client.getNick(), this->_name), this->_server);
            break;
        case NOSUCHCHANNEL:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NOSUCHCHANNEL(this->_server.getHostname(), this->_client.getNick(), this->_channels.front()), this->_server);
            break;
        case NOTONCHANNEL:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NOTONCHANNEL(this->_server.getHostname(), this->_client.getNick(), this->_channels.front()), this->_server);
            break;
        default:
            while (!this->_channels.empty()) {
                Channel* ch = this->_server.getChannels()[this->_channels.front()];
                ch->sendMessage(PART(this->_client.getNick(), this->_client.getUsername(), this->_client.getIpaddr(), this->_channels.front(), this->_message), 0);
                ch->isClient(this->_client.getSocketFD()) ? ch->removeClient(this->_client.getSocketFD()) : ch->removeOperator(this->_client.getSocketFD());
                if (ch->getClients().empty() && ch->getOperators().empty())
                    this->_server.removeChannel(this->_channels.front());
                else
                    this->_server.printChannelInfo(this->_channels.front());
                this->_channels.pop();
            }
            break;
    }
}

void Part::print() const{
    if (this->_error != 0)
        std::cout << " " << RED << "[" << this->_error << "]" << std::endl;
    else {
        std::cout << "\t[";
        showstringq(this->_channels);
        std::cout << "] [" << (this->_message.empty() ? "-" : this->_message) << "]" << std::endl;
    }
}