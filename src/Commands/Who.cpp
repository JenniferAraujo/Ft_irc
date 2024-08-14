#include "Includes.hpp"

Who::Who(Server& server, Client& client): ACommand("WHO", server, client) {};

void Who::parsing(std::istringstream &input){
	std::string channel;
    std::getline(input, channel, '\n');
    this->trimChar(channel, '\r');
    this->_channel = channel;
}

void Who::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << this->_name << std::endl;
    std::string msg, names;
    if (this->_server.getChannels().find(this->_channel) != this->_server.getChannels().end()) {
        Channel* channel = this->_server.getChannels()[this->_channel];
        std::cout << "SEG AQUI" << std::endl;
        std::cout << channel;
        std::map<int, Client*> clients = channel->getClients();
        std::cout << "SEG NÃO PASSA DAQUI" << std::endl;
        for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
            Client* c = it->second;
            names.append(c->getNick());
            names.append(" ");
            msg = RPL_WHO(this->_server.getHostname(), this->_channel, this->_client.getNick(), *c);
            send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
        }
    }
    msg = RPL_ENDWHO(this->_server.getHostname(), this->_channel, this->_client.getNick());
    send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
    msg = RPL_NAME(this->_server.getHostname(), this->_channel, this->_client.getNick(), names);
    send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
    msg = RPL_ENDNAME(this->_server.getHostname(), this->_channel, this->_client.getNick());
    send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
    send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
}

void Who::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << " | Channel: " << this->_channel << std::endl;
}