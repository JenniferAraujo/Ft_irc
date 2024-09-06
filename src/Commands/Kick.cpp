#include "Includes.hpp"

Kick::Kick(Server& server, Client& client): ACommand("KICK", server, client) {};

void Kick::parsing(std::istringstream &input) {
	std::string token;
    int n = 0;
    while (std::getline(input, token, ' ') && n < 3) {
        this->trimChar(token, '\r');
        std::cout << "token: " << token << std::endl;
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
                    this->_error = NOSUCHCHANNEL;     //KICK a
                    return ;
                }
                break;
            case 1:
                if (this->existentClient(token)) {
                    if (this->existentClientOnChannel(token, this->_channel))
                        this->_cliente = token;			//KICK #a Diogo
                }
                else {
                    this->_error = USERNOTINCHANNEL;
                    return ;
                }
                break;
            case 2:
                if (token[0] == ':') {
                    token.erase(token.begin());
                    this->_reason = token;
                    while (std::getline(input, token, ' ')) {
                        this->trimChar(token, '\r');
                        this->_reason.append(" ").append(token);			//KICK #a Diogo :idk
                    }
                }
                break;
        }
        n++;
    }
    if (this->_channel.empty() || this->_cliente.empty())
		this->_error = NEEDMOREPARAMS;
}

//TODO - Permissões de operador
//TODO - Remover o cliente do canal
//TODO - Tratamento de Erros
//FIXME - SEGFAUlT ao tentar dar auto-kick
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
        case USERNOTINCHANNEL:
            Message::sendMessage(this->_client.getSocketFD(), ERR_USERNOTINCHANNEL(this->_server.getHostname(), this->_client.getNick(), this->_channel), this->_server);
            break;
        default:
            Channel*    ch = this->_server.getChannels()[this->_channel];
            if (ch->isClient(this->_server.getClientByNick(this->_cliente)))
                ch->sendMessage(KICK(this->_client.getNick(), this->_client.getUsername(), this->_client.getIpaddr(), this->_channel, this->_cliente, this->_reason.empty() ? KICKDEFAULTMSG : this->_reason), 0);
            else
                std::cout << "Mensagens de erro" << std::endl;
            break;
    }

}

void Kick::print() const{
    // std::cout << "Command: " << this->_name <<  " | Error: " << this->_error;
    // std::cout << " | Channel: " << this->_channel << " | Client: " << this->_cliente;
    // if (!this->_reason.empty())
    //     std::cout << " | Reason: " << this->_reason;
    // std::cout << std::endl;
    if (this->_error != 0)
        std::cout << " " << RED << "[" << this->_error << "]" << std::endl;
    else
        std::cout << "\t[" << this->_channel << "] [" << this->_cliente << "] [" << (this->_reason.empty() ? "-" : this->_reason) << "]" << std::endl;
}