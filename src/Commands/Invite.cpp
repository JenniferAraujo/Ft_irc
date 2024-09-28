#include "Includes.hpp"

Invite::Invite(Server& server, Client& client): ACommand("INVITE", server, client) {};

void Invite::parsing(std::istringstream &input) {
	std::string token;
    int n = 0;
    while (std::getline(input, token, ' ') || n < 1 ) {
        trimChar(token, '\r');
        if (token.empty()) {
            this->_error = NEEDMOREPARAMS;      //INVITE  dinoguei
            return;
        }
        switch (n) {
            case 0: //INVITE dinoguei #a
                this->_nickname = token;
                if (this->_client.getNick() == token) {
                    this->_error = UNKNOWNERROR;        //AUTO-KICK
                    return ;
                }
                if (!this->existentClient(token)) {
                    this->_error = NOSUCHNICK;
                    return ;
                }
                break;
            case 1: //INVITE dinoguei #a
                this->_channel = token;
                if (token[0] != '#' || !this->existentChannel(token)) {
                    this->_error = NOSUCHCHANNEL;   //CANAL NÃO EXISTE || INVITE dinoguei a
                    return ;
                }
                if (!this->existentClientOnChannel(this->_client.getNick(), this->_channel)) {
                    this->_error = NOTONCHANNEL;        //NÃO ESTIVER NO CANAL
                    return;
                }
                break;
        }
        n++;
    }
    if (this->_error == 0) {
        if (this->_nickname.empty() || this->_channel.empty())
	    	this->_error = NEEDMOREPARAMS;
        else {
            Channel* ch = this->_server.getChannelLower(this->_channel);
            Client*     c = this->_server.getClients()[this->_server.getClientByNick(this->_nickname)];
            if (ch->isInviteOnly() && !ch->isOperator(this->_client.getSocketFD()))
                this->_error = CHANOPRIVSNEEDED;
            else if (ch->isClient(c->getSocketFD()) || ch->isOperator(c->getSocketFD()))
                this->_error = USERONCHANNEL;
        }
    }
}

void Invite::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << this->_name;
    this->print();
    switch (this->_error) {
        case NEEDMOREPARAMS:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NEEDMOREPARAMS(this->_server.getHostname(), this->_client.getNick(), this->_name), this->_server);
            break;
        case NOSUCHNICK:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NOSUCHNICK(this->_server.getHostname(), this->_client.getNick(), this->_nickname), this->_server);
            break;
        case NOSUCHCHANNEL:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NOSUCHCHANNEL(this->_server.getHostname(), this->_client.getNick(), this->_channel), this->_server);
            break;
        case NOTONCHANNEL:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NOTONCHANNEL(this->_server.getHostname(), this->_client.getNick(), this->_channel), this->_server);
            break;
        case USERONCHANNEL:
            Message::sendMessage(this->_client.getSocketFD(), ERR_USERONCHANNEL(this->_server.getHostname(), this->_client.getNick(), this->_nickname, this->_channel), this->_server);
            break;
        case CHANOPRIVSNEEDED:
            Message::sendMessage(this->_client.getSocketFD(), ERR_CHANOPRIVSNEEDED(this->_server.getHostname(), this->_client.getNick(), this->_channel), this->_server);
            break;
        case UNKNOWNERROR:
            Message::sendMessage(this->_client.getSocketFD(), ERR_UNKNOWNERROR(this->_server.getHostname(), this->_client.getNick(), this->_name, "You can't invite yourself to the channel"), this->_server);
            break;
        default:
            Channel* ch = this->_server.getChannelLower(this->_channel);
            Client*     c = this->_server.getClients()[this->_server.getClientByNick(this->_nickname)];
            Message::sendMessage(this->_client.getSocketFD(), RPL_INVITING(this->_server.getHostname(), this->_client.getNick(), this->_nickname, ch->getName()), this->_server);
            Message::sendMessage(c->getSocketFD(), INVITE(this->_client.getNick(), ch->getName(), this->_nickname), this->_server);
            ch->addInviteFD(c->getSocketFD());
            break;
    }
}

void Invite::print() const{
    if (this->_error != 0)
        std::cout << " " << RED << "[" << this->_error << "]" << std::endl;
    else
        std::cout << "\t[" << this->_nickname << "] [" << this->_channel << "]" << std::endl;
}