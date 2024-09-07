#include "Includes.hpp"

Invite::Invite(Server& server, Client& client): ACommand("INVITE", server, client) {};

void Invite::parsing(std::istringstream &input) {
    // USERONCHANNEL 443
    // CHANOPRIVNEEDED 482
	std::string token;
    int n = 0;
    while (std::getline(input, token, ' ') || n < 1 ) {
        trimChar(token, '\r');
        if (token.empty()) {
            this->_error = NEEDMOREPARAMS;      //INVITE  dinoguei
            return;
        }
        switch (n) {
            case 0:
                if (this->existentClient(token))
                    this->_nickname = token;      //INVITE dinoguei #a
                else {
                    this->_error = NOSUCHNICK;
                    return ;
                }
                std::cout << "case 0: " << token << std::endl;
                break;
            case 1:
                if (this->existentChannel(token))
                    this->_channel = token;     //INVITE dinoguei #a
                else {
                    this->_error = NOSUCHCHANNEL;
                    return ;
                }
                std::cout << "case 1: " << token << std::endl;
        }
        n++;
    }
    if (this->_nickname.empty() || this->_channel.empty())
		this->_error = NEEDMOREPARAMS;
}

//TODO - Handle de erros
//FIXME - SEGFAULT DE VEZ ENQUANDO, PENSO QUE É PK CONVIDO E A AUTENTICAÇÃO DO OUTRO USER AINDA NÃO FOI TERMINADA, MAS CONFIRMAR
void Invite::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << this->_name;
    this->print();
    switch (this->_error) {
        case NEEDMOREPARAMS:
             std::cout << "Erro bro 1" << std::endl;
            //Message::sendMessage(this->_client.getSocketFD(), ERR_NONICKNAMEGIVEN(this->_server.getHostname(), this->_client.getNick()), this->_server);
            break;
        case NOSUCHNICK:
             std::cout << "Erro bro 2" << std::endl;
            //Message::sendMessage(this->_client.getSocketFD(), ERR_ERRONEUSNICKNAME(this->_server.getHostname(), this->_client.getNick() , this->_nick), this->_server);
            break;
        case NOSUCHCHANNEL:
             std::cout << "Erro bro 3" << std::endl;
            //Message::sendMessage(this->_client.getSocketFD(), ERR_NICKNAMEINUSE(this->_server.getHostname(), this->_client.getNick(), this->_nick), this->_server);
            break;
        /*case USERONCHANNEL:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NICKNAMEINUSE(this->_server.getHostname(), this->_client.getNick(), this->_nick), this->_server);
            break;*/
        /*case CHANOPRIVNEEDED:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NICKNAMEINUSE(this->_server.getHostname(), this->_client.getNick(), this->_nick), this->_server);
            break;*/
        default:
            Channel*    ch = this->_server.getChannels()[this->_channel];
            Client*     c = this->_server.getClients()[this->_server.getClientByNick(this->_nickname)];
            Message::sendMessage(this->_client.getSocketFD(), RPL_INVITING(this->_server.getHostname(), this->_client.getNick(), this->_nickname, this->_channel), this->_server);
            Message::sendMessage(c->getSocketFD(), INVITE(this->_client.getNick(), this->_channel, this->_nickname), this->_server);
            ch->addInviteFD(c->getSocketFD());
            break;
    }
}

void Invite::print() const{
    //std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << " | Nickname: " << this->_nickname << " | Channel: " << this->_channel << std::endl;
    if (this->_error != 0)
        std::cout << " " << RED << "[" << this->_error << "]" << std::endl;
    else
        std::cout << "\t[" << this->_nickname << "] [" << this->_channel << "]" << std::endl;
}