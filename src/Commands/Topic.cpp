#include "Includes.hpp"

Topic::Topic(Server& server, Client& client): ACommand("TOPIC", server, client) {};

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
            case 0:
                if (token[0] == '#') {
                    if (this->existentChannel(token))
                        this->_channel = token;     //TOPIC #a
                    else {
                        this->_error = NOSUCHCHANNEL;
                        return ;
                    }
                }
                else {
                    this->_error = BADCHANMASK;     //TOPIC a
                    return ;
                }
                break;
            case 1:
                if (token[0] == ':') {
                    token.erase(token.begin());
                    if (token.empty())
                        this->_removeTopic = true;  //TOPIC #a :
                    else
                        this->_msg = token;			//TOPIC #a :Ajuda
                }
                break;
        }
        n++;
    }
}

//TODO - Permissões de Operador, Tratamento de erros
void Topic::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << this->_name << std::endl;
    if (this->_server.getChannels().find(this->_channel) != this->_server.getChannels().end()) {
        Channel* channel = this->_server.getChannels()[this->_channel];
        if (!this->_msg.empty()) {
            channel->setTopic(this->_msg);
            Message::sendMessage(this->_client.getSocketFD(), RPL_TOPIC(this->_server.getHostname(), this->_channel, this->_client.getNick(), channel->getTopic()), this->_server);
        } else if (this->_removeTopic) {
            channel->setTopic(NULL);
            // Confirmar se tenho de enviar isto
            Message::sendMessage(this->_client.getSocketFD(), RPL_NOTOPIC(this->_server.getHostname(), this->_channel, this->_client.getNick()), this->_server);
        } else {
            if (channel->getTopic().empty())
                Message::sendMessage(this->_client.getSocketFD(), RPL_NOTOPIC(this->_server.getHostname(), this->_channel, this->_client.getNick()), this->_server);
            else
                Message::sendMessage(this->_client.getSocketFD(), RPL_TOPIC(this->_server.getHostname(), this->_channel, this->_client.getNick(), channel->getTopic()), this->_server);
        }
    }
}

void Topic::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << " | Channel: " << this->_channel << " | Msg: " << this->_msg << " | Remove Topic: " << this->_removeTopic << std::endl;
}