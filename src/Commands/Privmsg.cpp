#include "Includes.hpp"

Privmsg::Privmsg(Server& server, Client& client): ACommand("PRIVMSG", server, client) {};

//Command: PRIVMSG
//Parameters: <target>{,<target>} <text to be sent>
//Syntax: PRIVMSG <target>{,<target>} :<text to be sent>

/* # define TOOMANYTARGETS     407 check
# define NORECIPIENT        411 amanha PRIVMSG :ola tudo bem
# define NOTEXTTOSEND       412 */ //check
void Privmsg::parsingToken(std::string token) {
        if (token[0] == '#')
            this->_channels.push(token); //#c
        else if (token[0] == '@' && token[1] == '#')
            this->_opChannels.push(token); //@#c
        else
            this->_clients.push(token); //c
}

void Privmsg::parsing(std::istringstream &input){
	std::string token;
    std::getline(input, token, ' ');
    trimChar(token, '\r');
    if (token.empty()) {
        this->_error = NEEDMOREPARAMS;  //PRIVMSG -> check 
            return;
    }
    if(token[0] == ':'){
        this->_error = NORECIPIENT; //PRIVMSG :ola tudo bem -> check
        return ;
    }
    int count = std::count(token.begin(), token.end(), ',');
    if (count == 0)
		this->parsingToken(token); //PRIVMSG rita :ola tudo bem || //PRIVMSG #c :ola tudo bem || //PRIVMSG @#c :ola tudo bem -> check
    else {
        std::istringstream tokenstream(token);
        while (std::getline(tokenstream, token, ',')) {
            trimChar(token, '\r');
			this->parsingToken(token);  //PRIVMSG rita,#c,@#c :ola tudo bem -> check
        }
    }
    std::getline(input, this->_message);
    if(this->_message.empty() || this->_message[0] != ':')
        this->_error = NEEDMOREPARAMS; //erro de formataçao, sem os : antes da msg: PRIVMSG #c ola || PRIVMSG #c -> check
	else if (this->_message.size() == 1)
		this->_error = NOTEXTTOSEND; //sem msg: PRIVMSG #c : //FIXME
    else if(this->_channels.size() + this->_opChannels.size() + this->_clients.size() > MAX_TARGETS)
        this->_error = TOOMANYTARGETS;
}

/* void Privmsg::sendToOpChannels() 
void Privmsg::sendToChannels()
void Privmsg::sendToClients() */

//só podes mandar msg a alguem no mesmo canal que tu? nao
//podes mandar msg para ti mesmo? sim
//podes misturar tipos de target: sim
//mandar msg para canal, é para todos menos para ti mesmo
//NOSUCHNICK     401 //execuçao
//NOSUCHCHANNEL  403 //execuçao
void Privmsg::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "");
    switch (this->_error) {
        case NEEDMOREPARAMS:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NEEDMOREPARAMS(this->_server.getHostname(), this->_client.getNick(), this->_name), this->_server);
            break;
        case NORECIPIENT:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NORECIPIENT(this->_server.getHostname(), this->_client.getNick(), this->_name), this->_server);
            break;
        case NOTEXTTOSEND:
            Message::sendMessage(this->_client.getSocketFD(), ERR_NOTEXTTOSEND(this->_server.getHostname(), this->_client.getNick()), this->_server);
            break;
        case TOOMANYTARGETS:
            Message::sendMessage(this->_client.getSocketFD(), ERR_TOOMANYTARGETS(this->_server.getHostname(), this->_client.getNick(), this->_name), this->_server);
            break;
        default:
            this->print();
            break;
    }
}

void Privmsg::print() const{
    std::cout << this->_name <<  "\t[";
    showstringq(this->_channels);
    std::cout << "] [";
    showstringq(this->_opChannels);
    std::cout << "] [";
    showstringq(this->_clients);
    std::cout << "]\n";
}
