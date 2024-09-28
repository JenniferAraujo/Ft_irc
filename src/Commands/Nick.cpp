#include "Includes.hpp"

Nick::Nick(Server& server, Client& client): ACommand("NICK", server, client) {}

void Nick::parsing(std::istringstream &input){
    std::getline(input, this->_nick);
    if (this->_nick.empty()){
        _error = NONICKNAMEGIVEN; //NICK
        return ;
    }
    trimChar(this->_nick, '\r');
    if (this->_nick.find(' ') != std::string::npos || this->_nick.find('\t') != std::string::npos 
        || this->_nick.find('"') != std::string::npos || this->_nick.find(39) != std::string::npos
            || this->_nick.find('#') != std::string::npos || this->_nick.find('&') != std::string::npos
                || this->_nick.find(':') != std::string::npos || this->_nick.find(',') != std::string::npos
                    || this->_nick.find(';') != std::string::npos || this->_nick.find('*') != std::string::npos
                        || this->_nick.find('?') != std::string::npos || this->_nick.find('!') != std::string::npos
                            || this->_nick.find('@') != std::string::npos)
                        _error = ERRONEUSNICKNAME; 
                        //NICK rita a linda || NICK rita a   linda || NICK  rit"a || NICK 'rita
                        //NICK #rita || NICK &#rita || NICK  #&rita || NICK :rita
                        //NICK rita; || NICK rit,a 
    else if (this->_nick.length() > MAXCHARS)
        _error = ERRONEUSNICKNAME; //NICK nomedemasiadocomprido
    else if(this->_server.findClient(this->_nick, this->_client.getSocketFD()) != NULL 
            && this->_server.findClient(this->_nick, this->_client.getSocketFD())->getRegistration())//NICK nick_repetido
        _error = NICKNAMEINUSE;
}

void Nick::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << this->_name;
    this->print();
    switch (this->_error) {
        case NONICKNAMEGIVEN:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NONICKNAMEGIVEN(this->_server.getHostname(), this->_client.getNick()), this->_server);
            break;
        case ERRONEUSNICKNAME:
            Message::sendMessage(this->_client.getSocketFD(), ERR_ERRONEUSNICKNAME(this->_server.getHostname(), this->_client.getNick() , this->_nick), this->_server);
            break;
        case NICKNAMEINUSE:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NICKNAMEINUSE(this->_server.getHostname(), this->_client.getNick(), this->_nick), this->_server);
            break;
        default:
            Message::sendMessage(this->_client.getSocketFD(), NICK_MESSAGE(this->_client.getNick(), this->_client.getUsername(), 
                this->_client.getIpaddr(), this->_nick), this->_server);
            if(this->_client.getRegistration()){
                std::map<int, Client*> clientsOnSameChannel = this->_server.getClientsOnSameChannel(this->_client.getSocketFD());
                this->_server.sendMsg(clientsOnSameChannel, NICK_MESSAGE(this->_client.getNick(), this->_client.getUsername(), 
                    this->_client.getIpaddr(), this->_nick));
                clientsOnSameChannel.clear();

            }
            this->_client.setNick(this->_nick);
            break;
    }
}

void Nick::print() const{
    //std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << " | Nick: " << this->_nick << std::endl;
    if (this->_error != 0)
        std::cout << " " << RED << "[" << this->_error << "]" << std::endl;
    else
        std::cout << "\t[" << this->_nick << "]" << std::endl;
}