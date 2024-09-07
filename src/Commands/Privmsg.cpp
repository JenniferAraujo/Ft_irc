#include "Includes.hpp"

Privmsg::Privmsg(Server& server, Client& client): ACommand("PRIVMSG", server, client) {};

//Command: PRIVMSG
//Parameters: <target>{,<target>} <text to be sent>
//Syntax: PRIVMSG <target>{,<target>} :<text to be sent>

/* # define TOOMANYTARGETS     407 check
# define NORECIPIENT        411 amanha PRIVMSG :ola tudo bem
# define NOTEXTTOSEND       412 */ //check
void Privmsg::parsingToken(std::string token) {
        if (token[0] == '#'){
            if(!findInQueue(this->_channels, token))
                this->_channels.push(token); //#c
        }
        else if (token[0] == '@' && token[1] == '#'){
            if(!findInQueue(this->_opChannels, token))
                this->_opChannels.push(token); //@#c
        }
        else{
            if(!findInQueue(this->_clients, token))
                this->_clients.push(token); //c
        }
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
		this->_error = NOTEXTTOSEND; //sem msg: PRIVMSG #c : -> check
    else if(this->_channels.size() + this->_opChannels.size() + this->_clients.size() > MAX_TARGETS)
        this->_error = TOOMANYTARGETS;
}

//podes misturar tipos de target: sim
//mandar msg para canal, é para todos menos para ti mesmo -> done -> check
//NOSUCHCHANNEL  403 //execuçao -> done -> check
void Privmsg::sendToChannels(){
    while (!this->_channels.empty()) {
        std::string channelName = this->_channels.front();
        if(existentChannel(channelName)){
            Channel *channel = this->_server.getChannels()[channelName];
            if(!channel->isClient(this->_client.getSocketFD()) && !channel->isOperator(this->_client.getSocketFD())){
                Message::sendMessage(this->_client.getSocketFD(), ERR_CANNOTSENDTOCHAN(this->_server.getHostname(), this->_client.getNick(), channelName), this->_server);
            }
            else
                channel->sendMessage(PRIV_MESSAGE(this->_client.getNick(), this->_client.getUsername(), 
                    this->_client.getIpaddr(), channelName, this->_message), this->_client.getSocketFD());
        }
        else{
            Message::sendMessage(this->_client.getSocketFD(), ERR_NOSUCHCHANNEL(this->_server.getHostname(), this->_client.getNick(), channelName), this->_server);
        }
        this->_channels.pop();
    }
}

void Privmsg::sendToOpChannels(){
    while (!this->_opChannels.empty()) {
        std::string channelName = this->_opChannels.front().substr(1, this->_opChannels.front().length() - 1);
        if(existentChannel(channelName)){
            Channel *channel = this->_server.getChannels()[channelName];
            if(!channel->isClient(this->_client.getSocketFD()) && !channel->isOperator(this->_client.getSocketFD())){
                Message::sendMessage(this->_client.getSocketFD(), ERR_CANNOTSENDTOCHAN(this->_server.getHostname(), this->_client.getNick(), channelName), this->_server);
            }
            else
                channel->sendMessageToOperators( PRIV_MESSAGE(this->_client.getNick(), this->_client.getUsername(), 
                    this->_client.getIpaddr(), channelName, this->_message), this->_client.getSocketFD());
        }
        else
            Message::sendMessage(this->_client.getSocketFD(), ERR_NOSUCHCHANNEL(this->_server.getHostname(), this->_client.getNick(), channelName), this->_server);
        _opChannels.pop();
    }
}

void Privmsg::sendToClients(){
    while (!this->_clients.empty()) {
        std::string nick = this->_clients.front();
        if(existentClient(nick)){
            Message::sendMessage(this->_server.getClientByNick(nick), PRIV_MESSAGE(this->_client.getNick(), this->_client.getUsername(), 
                    this->_client.getIpaddr(), nick, this->_message), this->_server);
        }
        else
            Message::sendMessage(this->_client.getSocketFD(), ERR_NOSUCHNICK(this->_server.getHostname(), this->_client.getNick(), nick), this->_server);
        this->_clients.pop();
    }
}

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
            this->sendToChannels();
            this->sendToOpChannels();
            this->sendToClients();
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
    std::cout << "] [" << this->_message << "]\n";
}
