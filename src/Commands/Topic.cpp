#include "Includes.hpp"

Topic::Topic(Server& server, Client& client): ACommand("TOPIC", server, client) {};

//TODO - VERIFICAR O TOPICLOCKED E SE É OPERADOR
void Topic::parsing(std::istringstream &input){
	std::string token;
    int n = 0;
    while (std::getline(input, token, ' ') || n < 2) {
        trimChar(token, '\r');
        if (token.empty()) {
            this->_error = NEEDMOREPARAMS;          //TOPIC  #a
            return;
        }
        switch (n)
        {
            case 0: //TOPIC #a
                this->_channel = token;
                if (token[0] != '#' || !this->existentChannel(token)) {
                    this->_error = NOSUCHCHANNEL;   //CANAL NÃO EXISTE || KICK a
                    return ;
                }
                break;
            case 1: //TOPIC #a : || TOPIC #a :Ajuda no Chat
                if (token[0] == ':') {
                    token.erase(token.begin());
                    if (token.empty()) {
                        this->_removeTopic = true;
                        return;
                    }
                    this->_msg = token;
                    while (std::getline(input, token, ' ')) {
                        trimChar(token, '\r');
                        this->_msg.append(" ").append(token);
                    }
                }
                break;
        }
        n++;
    }
    if (this->_error == 0) {
        if (this->_channel.empty())
            this->_error = NEEDMOREPARAMS;
        else if (!this->existentClientOnChannel(this->_client.getNick(), this->_channel)) {
            this->_error = NOTONCHANNEL;        //NÃO ESTIVER NO CANAL
        } else {
            Channel* ch = this->_server.getChannelLower(this->_channel);
            if (ch->isTopicLocked() && !ch->isOperator(this->_client.getSocketFD()) && !this->_msg.empty())
                this->_error = CHANOPRIVSNEEDED;
        }
    }
}

//TODO - Tratamento de erros
void Topic::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << this->_name;
    this->print();
    switch (this->_error) {
        case NEEDMOREPARAMS:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NEEDMOREPARAMS(this->_server.getHostname(), this->_client.getNick(), this->_name), this->_server);
            break;
        case NOSUCHCHANNEL:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NOSUCHCHANNEL(this->_server.getHostname(), this->_client.getNick(), this->_channel), this->_server);
            break;
        case NOTONCHANNEL:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NOTONCHANNEL(this->_server.getHostname(), this->_client.getNick(), this->_channel), this->_server);
            break;
        case CHANOPRIVSNEEDED:
            Message::sendMessage(this->_client.getSocketFD(), ERR_CHANOPRIVSNEEDED(this->_server.getHostname(), this->_client.getNick(), this->_channel), this->_server);
            break;
        default:
            Channel* ch = this->_server.getChannelLower(this->_channel);
            if (!this->_msg.empty()) {
                ch->setTopic(this->_msg);
                Message::sendMessage(this->_client.getSocketFD(), RPL_TOPIC(this->_server.getHostname(), ch->getName(), this->_client.getNick(), ch->getTopic()), this->_server);
            } else if (this->_removeTopic) {
                ch->setTopic("");
                // Confirmar se tenho de enviar isto
                Message::sendMessage(this->_client.getSocketFD(), RPL_NOTOPIC(this->_server.getHostname(), ch->getName(), this->_client.getNick()), this->_server);
            } else {
                if (ch->getTopic().empty())
                    Message::sendMessage(this->_client.getSocketFD(), RPL_NOTOPIC(this->_server.getHostname(), ch->getName(), this->_client.getNick()), this->_server);
                else
                    Message::sendMessage(this->_client.getSocketFD(), RPL_TOPIC(this->_server.getHostname(), ch->getName(), this->_client.getNick(), ch->getTopic()), this->_server);
            }
            break;
    }
}

void Topic::print() const{
    if (this->_error != 0)
        std::cout << " " << RED << "[" << this->_error << "]" << std::endl;
    else
        std::cout << "\t[" << this->_channel << "] [" << this->_msg << "] [" << (this->_removeTopic ? "Removing Topic" : "-") << "]" << std::endl;
}