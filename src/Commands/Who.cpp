#include "Includes.hpp"

Who::Who(Server& server, Client& client): ACommand("WHO", server, client) {};

void Who::parsing(std::istringstream &input){
	std::string channel;
    std::getline(input, channel, '\n');
    this->trimChar(channel, '\r');
    this->_channel = channel;
	//std::cout << "channel:" << this->_channel << std::endl;
}

void Who::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << this->_name << std::endl;
    std::string msg, names;
    if (this->_server.getChannels().find(this->_channel) != this->_server.getChannels().end()) {
        Channel* channel = this->_server.getChannels()[this->_channel];
        std::map<int, Client*> clients = channel->getClients();
        for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
            Client* c = it->second;
            names.append(c->getNick()).append(" ");
            Message::sendMessage(this->_client.getSocketFD(), RPL_WHO(this->_server.getHostname(), this->_channel, this->_client.getNick(), "", *c), this->_server);
        }
        std::map<int, Client*> op = channel->getOperators();
        for (std::map<int, Client*>::iterator it = op.begin(); it != op.end(); ++it) {
            Client* c = it->second;
            names.append("@").append(c->getNick()).append(" ");
            Message::sendMessage(this->_client.getSocketFD(), RPL_WHO(this->_server.getHostname(), this->_channel, this->_client.getNick(), "@", *c), this->_server);
        }
        Message::sendMessage(this->_client.getSocketFD(), RPL_ENDWHO(this->_server.getHostname(), this->_channel, this->_client.getNick()), this->_server);
        if (this->_client.getJustJoined()) {
            Message::sendMessage(this->_client.getSocketFD(), RPL_NAME(this->_server.getHostname(), this->_channel, this->_client.getNick(), names), this->_server);
            Message::sendMessage(this->_client.getSocketFD(), RPL_ENDNAME(this->_server.getHostname(), this->_channel, this->_client.getNick()), this->_server);
            this->_client.setJustJoined(false);
        }
    }
    else {
        std::cout << "Não entrou burro" << std::endl;
    }
}

void Who::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << " | Channel: " << this->_channel << std::endl;
}