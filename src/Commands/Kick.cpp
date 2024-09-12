#include "Includes.hpp"

Kick::Kick(Server& server, Client& client): ACommand("KICK", server, client) {};

void Kick::parsing(std::istringstream &input) {
	std::string token;
    int n = 0;
    while (std::getline(input, token, ' ') || n < 3) {
        trimChar(token, '\r');
        if (token.empty()) {
            this->_error = NEEDMOREPARAMS;          //KICK  #a Diogo
            return;
        }
        switch (n)
        {
            case 0: //KICK #a Diogo
                this->_channel = token;
                if (token[0] != '#' || !this->existentChannel(token)) {
                        this->_error = NOSUCHCHANNEL;   //CANAL NÃO EXISTE || KICK a
                        return ;
                }
                break;
            case 1: //KICK #a Diogo
                this->_cliente = token;
                if (this->_client.getNick() == token) {
                    this->_error = UNKNOWNERROR;        //AUTO-KICK
                    return ;
                }
                if (!this->existentClient(token)) {
                    this->_error = NOSUCHNICK;          //CLIENTE NÃO EXISTE
                    return ;
                }
                if (!this->existentClientOnChannel(this->_client.getNick(), this->_channel)) {
                    this->_error = NOTONCHANNEL;        //NÃO ESTIVER NO CANAL
                    return;
                }
                //NOTE - Verificar a condição de não deixar kickar operadores
                if (!this->existentClientOnChannel(token, this->_channel)) {
                    Channel*    ch = this->_server.getChannels()[this->_channel];
                    Client*     c = this->_server.findClient(this->_cliente, 0);
                    this->_error = ch->isOperator(c->getSocketFD()) ? UNKNOWNERROR : USERNOTINCHANNEL; //NÃO PODE KICKAR OPERADORES || CLIENTE NÃO ESTÁ NO CANAL
                    return ;
                }
                break;
            case 2: //KICK #a Diogo :idk
                if (token[0] == ':') {
                    token.erase(token.begin());
                    this->_reason = token;
                    while (std::getline(input, token, ' ')) {
                        trimChar(token, '\r');
                        this->_reason.append(" ").append(token);
                    }
                }
                break;
        }
        n++;
    }
    if (this->_error == 0) {
        if (this->_channel.empty() || this->_cliente.empty())
            this->_error = NEEDMOREPARAMS;
        else { 
            Channel* ch = this->_server.getChannels()[this->_channel];
            if (!ch->isOperator(this->_client.getSocketFD()))
                this->_error = CHANOPRIVSNEEDED;
        }
    }
}

void Kick::execute() {
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
            Message::sendMessage(this->_client.getSocketFD(), ERR_NOSUCHNICK(this->_server.getHostname(), this->_client.getNick(), this->_cliente), this->_server);
            break;
        case USERNOTINCHANNEL:
            Message::sendMessage(this->_client.getSocketFD(), ERR_USERNOTINCHANNEL(this->_server.getHostname(), this->_client.getNick(), this->_cliente, this->_channel), this->_server);
            break;
        case NOTONCHANNEL:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NOTONCHANNEL(this->_server.getHostname(), this->_client.getNick(), this->_channel), this->_server);
            break;
        case CHANOPRIVSNEEDED:
            Message::sendMessage(this->_client.getSocketFD(), ERR_CHANOPRIVSNEEDED(this->_server.getHostname(), this->_client.getNick(), this->_channel), this->_server);
            break;
        case UNKNOWNERROR:
            Message::sendMessage(this->_client.getSocketFD(), ERR_UNKNOWNERROR(this->_server.getHostname(), this->_client.getNick(), this->_name, "You can't kick yourself, or kick channel operators"), this->_server);
            break;
        default:
            Channel*    ch = this->_server.getChannels()[this->_channel];
            Client*     c = this->_server.findClient(this->_cliente, 0);
            ch->sendMessage(KICK(this->_client.getNick(), this->_client.getUsername(), this->_client.getIpaddr(), this->_channel, this->_cliente, this->_reason.empty() ? KICKDEFAULTMSG : this->_reason), 0);
            ch->removeClient(c->getSocketFD());
            this->_server.printChannelInfo(this->_channel);
            break;
    }
}

void Kick::print() const{
    if (this->_error != 0)
        std::cout << " " << RED << "[" << this->_error << "]" << std::endl;
    else
        std::cout << "\t[" << this->_channel << "] [" << this->_cliente << "] [" << (this->_reason.empty() ? "-" : this->_reason) << "]" << std::endl;
}