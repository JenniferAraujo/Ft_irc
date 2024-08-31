#include "Includes.hpp"

Nick::Nick(Server& server, Client& client): ACommand("NICK", server, client) {}

void Nick::parsing(std::istringstream &input){
    std::getline(input, this->_nick);
    if (this->_nick.empty()){
        _error = NONICKNAMEGIVEN; //NICK
        return ;
    }
    this->trimChar(this->_nick, '\r');
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

//TODO - Send message to clients - May
//The NICK message may be sent from the server to clients to acknowledge their NICK command was successful, and to inform other clients about the change of nickname. In these cases, the <source> of the message will be the old nickname [ [ "!" user ] "@" host ] of the user who is changing their nickname.
void Nick::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << RESET << this->_name << std::endl;
    std::string msg;
    switch (this->_error) {
        case NONICKNAMEGIVEN:
            if (this->_client.getNick().empty())
                msg.append(ERR_NONICKNAMEGIVEN(this->_server.getHostname(), "*"));
            else
                msg.append(ERR_NONICKNAMEGIVEN(this->_server.getHostname(), this->_client.getNick()));
            break;
        case ERRONEUSNICKNAME:
            if (this->_client.getNick().empty())
                msg.append(ERR_ERRONEUSNICKNAME(this->_server.getHostname(), "*" , this->_nick));
            else
                msg.append(ERR_ERRONEUSNICKNAME(this->_server.getHostname(), this->_client.getNick(), this->_nick));
            break;
        case NICKNAMEINUSE:
            if (this->_client.getNick().empty())
                msg.append(ERR_NICKNAMEINUSE(this->_server.getHostname(), "*" , this->_nick));
            else
                msg.append(ERR_NICKNAMEINUSE(this->_server.getHostname(), this->_client.getNick(), this->_nick));
            break;
        default:
            this->_client.setNick(this->_nick);
            break;
    }
    if(!msg.empty())
        send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
}

void Nick::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << " | Nick: " << this->_nick << std::endl;
}