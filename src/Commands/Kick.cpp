#include "Includes.hpp"

Kick::Kick(Server& server, Client& client): ACommand("KICK", server, client) {};

void Kick::parsing(std::istringstream &input) {
	std::string token;
    int n = 0;
    while (std::getline(input, token, ' ') || n < 3) {
        this->trimChar(token, '\r');
        if (token.empty()) {
            this->_error = NEEDMOREPARAMS;          //KICK  #a Diogo
            return;
        }
        switch (n)
        {
            case 0:
                if (token[0] == '#') {
                    if (this->existentChannel(token))
                        this->_channel = token;     //KICK #a Diogo
                    else {
                        this->_error = NOSUCHCHANNEL;
                        return ;
                    }
                }
                else {
                    this->_error = BADCHANMASK;     //KICK a
                    return ;
                }
                break;
            case 1:
                if (this->existentClient(token))
                    this->_cliente = token;			//KICK #a Diogo
                else {
                    this->_error = USERNOTINCHANNEL;
                    return ;
                }
                break;
            case 2:
            //NOTE - CONFIRMAR SE NÃO FUDI PARA KICK #a Diogo idk
                if (token[0] == ':') {
                    token.erase(token.begin());
                    this->_reason = token;			//KICK #a Diogo :idk
                }
                break;
        }
        n++;
    }
    if (this->_channel.empty() || this->_cliente.empty())
		this->_error = NEEDMOREPARAMS;
}

void Kick::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << this->_name << std::endl;
    //TODO - Permissões de OPERADOR
    //TODO - FALTA RETIRAR DO MAP
    if (this->_server.getChannels().find(this->_channel) != this->_server.getChannels().end()) {
        Channel* channel = this->_server.getChannels()[this->_channel];
        if (this->_error == 0 ) {
            if (channel->isClient(this->_server.getClientByNick(this->_cliente)))
                channel->sendMessage(KICK(this->_client.getNick(), this->_client.getUsername(), this->_client.getIpaddr(), this->_channel, this->_cliente, this->_reason.empty() ? KICKDEFAULTMSG : this->_reason), 0);
            else
                std::cout << "Mensagens de erro" << std::endl;
        } else {
                std::cout << "Mensagens de erro" << std::endl;
        }
    }

}

void Kick::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error;
    std::cout << " | Channel: " << this->_channel << " | Client: " << this->_cliente;
    if (!this->_reason.empty())
        std::cout << " | Reason: " << this->_reason;
    std::cout << std::endl;
}