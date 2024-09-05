#include "Includes.hpp"

Privmsg::Privmsg(Server& server, Client& client): ACommand("PRIVMSG", server, client) {};

//Command: PRIVMSG
//Parameters: <target>{,<target>} <text to be sent>
//Syntax: PRIVMSG <target>{,<target>} :<text to be sent>

/* # define TOOMANYTARGETS     407 check
# define NORECIPIENT        411 amanha
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
    this->trimChar(token, '\r');
    if (token.empty()) {
        this->_error = NEEDMOREPARAMS;  //Privmsg  #a
            return;
    }
    int count = std::count(token.begin(), token.end(), ',');
    if (count == 0)
		this->parsingToken(token);
    else {
        std::istringstream tokenstream(token);
        while (std::getline(tokenstream, token, ',')) {
            this->trimChar(token, '\r');
			this->parsingToken(token);
        }
    }
    std::getline(input, this->_message);
    if(this->_message.empty() || this->_message[0] != ':')
        this->_error = NEEDMOREPARAMS; //erro de formataçao, sem os : antes da msg: PRIVMSG #c ola || PRIVMSG #c
	else if (this->_message.size() == 1)
		this->_error = NOTEXTTOSEND; //sem msg: PRIVMSG #c :
    else if(this->_channels.size() + this->_opChannels.size() + this->_clients.size() > MAX_TARGETS)
        this->_error = TOOMANYTARGETS;
}

/* void Privmsg::sendToOpChannels() 
void Privmsg::sendToChannels()
void Privmsg::sendToClients() */

//so podes mandar msg a alguem no mesmo canal que tu? nao
//podes mandar msg para ti mesmo? sim
//podes misturar tipos de target: sim
//mandar msg para canal, é para todos menos para ti mesmo
//NOSUCHNICK     401 //execuçao
//NOSUCHCHANNEL  403 //execuçao
void Privmsg::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "");
    this->print();
}

void Privmsg::print() const{
    std::cout << this->_name <<  "\t";
    showstringq(this->_channels);
    showstringq(this->_opChannels);
    showstringq(this->_clients);
}
