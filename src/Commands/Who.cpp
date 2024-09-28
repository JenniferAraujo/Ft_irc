#include "Includes.hpp"

Who::Who(Server& server, Client& client): ACommand("WHO", server, client) { _isNick = false; };

void Who::parsing(std::istringstream &input){
	std::string token;
    int n = 0;
    while (std::getline(input, token, ' ') || n < 1) {
        trimChar(token, '\r');
        if (token.empty()) {
            this->_error = NEEDMOREPARAMS;          //WHO  #a
            return;
        }
        switch (n)
        {
            case 0: //WHO #a
                this->_channel = token;
                if (token[0] == '#') {
                    if (!this->existentChannel(token)) {
                        this->_error = NOSUCHCHANNEL;   //CANAL NÃO EXISTE
                        return ;
                    }
                } else {
                    if (!this->existentClient(token)) {
                        this->_error = NOSUCHNICK;   //USER NÃO EXISTE
                        return ;
                    }
                    this->_isNick = true;
                }
            break;
        }
        n++;
    }
    if (this->_error == 0) {
        if (this->_channel.empty())
            this->_error = NEEDMOREPARAMS;
    }
}

void Who::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << this->_name;
    this->print();
    switch (this->_error) {
        case NEEDMOREPARAMS:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NEEDMOREPARAMS(this->_server.getHostname(), this->_client.getNick(), this->_name), this->_server);
            break;
        case NOSUCHCHANNEL:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NOSUCHCHANNEL(this->_server.getHostname(), this->_client.getNick(), this->_channel), this->_server);
            break;
        case NOSUCHNICK:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NOSUCHNICK(this->_server.getHostname(), this->_client.getNick(), this->_channel), this->_server);
            break;
        default:
            if (!this->_isNick) {
                std::string names;
                Channel* channel = this->_server.getChannelLower(this->_channel);
                if (channel != NULL) {
                    std::map<int, Client*> clients = channel->getClients();
                    for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
                        Client* c = it->second;
                        names.append(c->getNick()).append(" ");
                        Message::sendMessage(this->_client.getSocketFD(), RPL_WHO(this->_server.getHostname(), channel->getName(), this->_client.getNick(), "", *c), this->_server);
                    }
                    std::map<int, Client*> op = channel->getOperators();
                    for (std::map<int, Client*>::iterator it = op.begin(); it != op.end(); ++it) {
                        Client* c = it->second;
                        names.append("@").append(c->getNick()).append(" ");
                        Message::sendMessage(this->_client.getSocketFD(), RPL_WHO(this->_server.getHostname(), channel->getName(), this->_client.getNick(), "@", *c), this->_server);
                    }
                    Message::sendMessage(this->_client.getSocketFD(), RPL_ENDWHO(this->_server.getHostname(), channel->getName(), this->_client.getNick()), this->_server);
                    if (this->_client.getJustJoined(channel->getName())) {
                        Message::sendMessage(this->_client.getSocketFD(), RPL_NAME(this->_server.getHostname(), channel->getName(), this->_client.getNick(), names), this->_server);
                        Message::sendMessage(this->_client.getSocketFD(), RPL_ENDNAME(this->_server.getHostname(), channel->getName(), this->_client.getNick()), this->_server);
                        this->_client.setJustJoined(false, channel->getName());
                    }
                }
            } else {
                int cFD = this->_server.getClientByNick(this->_channel);
                Client *c = this->_server.getClients()[cFD];
                Message::sendMessage(this->_client.getSocketFD(), RPL_WHO(this->_server.getHostname(), "*", this->_client.getNick(), "", *c), this->_server);
            }
    }
}

void Who::print() const{
    if (this->_error != 0)
        std::cout << " " << RED << "[" << this->_error << "]" << std::endl;
    else
        std::cout << "\t[" << this->_channel << "]" << std::endl;
}