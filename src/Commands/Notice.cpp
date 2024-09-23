#include "Includes.hpp"

Notice::Notice(Server& server, Client& client): ACommand("NOTICE", server, client) {};

void Notice::parsingToken(std::string token) {
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

void Notice::parsing(std::istringstream &input){
	std::string token;
    std::getline(input, token, ' ');
    trimChar(token, '\r');
    if (token.empty()) {
        this->_error = NEEDMOREPARAMS;  //Notice -> check 
        return;
    }
    if(token[0] == ':'){
        this->_error = NORECIPIENT; //Notice :ola tudo bem -> check
        return ;
    }
    int count = std::count(token.begin(), token.end(), ',');
    if (count == 0)
		this->parsingToken(token); //Notice rita :ola tudo bem || //Notice #c :ola tudo bem || //Notice @#c :ola tudo bem -> check
    else {
        std::istringstream tokenstream(token);
        while (std::getline(tokenstream, token, ',')) {
            trimChar(token, '\r');
			this->parsingToken(token);  //Notice rita,#c,@#c :ola tudo bem -> check
        }
    }
    std::getline(input, this->_message);
    if(this->_message.empty() || this->_message[0] != ':')
        this->_error = NEEDMOREPARAMS; //erro de formataçao, sem os : antes da msg: Notice #c ola || Notice #c -> check
	else if (this->_message.size() == 1)
		this->_error = NOTEXTTOSEND; //sem msg: Notice #c : -> check
    else if(this->_channels.size() + this->_opChannels.size() + this->_clients.size() > MAX_TARGETS)
        this->_error = TOOMANYTARGETS;
}

//podes misturar tipos de target: sim
//mandar msg para canal, é para todos menos para ti mesmo -> done -> check
//NOSUCHCHANNEL  403 //execuçao -> done -> check
void Notice::sendToChannels(){
    while (!this->_channels.empty()) {
        std::string channelName = this->_channels.front();
        if(existentChannel(channelName)){
            Channel *channel = this->_server.getChannels()[channelName];
            if(channel->isClient(this->_client.getSocketFD()) || channel->isOperator(this->_client.getSocketFD()))
                channel->sendMessage(NOTICE(this->_client.getNick(), this->_client.getUsername(), 
                    this->_client.getIpaddr(), channelName, this->_message), this->_client.getSocketFD());
        }
        this->_channels.pop();
    }
}

void Notice::sendToOpChannels(){
    while (!this->_opChannels.empty()) {
        std::string channelName = this->_opChannels.front().substr(1, this->_opChannels.front().length() - 1);
        if(existentChannel(channelName)){
            Channel *channel = this->_server.getChannels()[channelName];
            if(channel->isClient(this->_client.getSocketFD()) || channel->isOperator(this->_client.getSocketFD()))
                channel->sendMessageToOperators(NOTICE(this->_client.getNick(), this->_client.getUsername(), 
                    this->_client.getIpaddr(), channelName, this->_message), this->_client.getSocketFD());
        }
        _opChannels.pop();
    }
}

void Notice::sendToClients(){
    while (!this->_clients.empty()) {
        std::string nick = this->_clients.front();
        if(existentClient(nick)){
            Message::sendMessage(this->_server.getClientByNick(nick), NOTICE(this->_client.getNick(), this->_client.getUsername(), 
                    this->_client.getIpaddr(), nick, this->_message), this->_server);
        }
        this->_clients.pop();
    }
}

void Notice::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "");
    if(this->_error)
        return;
    this->print();
    this->sendToChannels();
    this->sendToOpChannels();
    this->sendToClients();
}

void Notice::print() const{
    std::cout << this->_name <<  "\t[";
    showstringq(this->_channels);
    std::cout << "] [";
    showstringq(this->_opChannels);
    std::cout << "] [";
    showstringq(this->_clients);
    std::cout << "] [" << this->_message << "]\n";
}
