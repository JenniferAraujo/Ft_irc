#include "Includes.hpp"

bool ACommand::isRegistration(){
    std::string commands[] = {"CAP", "PASS", "NICK", "USER"};
    for (int i = 0; i < 4; i++) {
        if(this->getName() == commands[i])
            return true;
    }
    return false;
}

bool ACommand::existentChannel(std::string channelName) {
    std::map<std::string, Channel *> channels = this->_server.getChannels();
    std::map<std::string, Channel *>::iterator it;
    for (it = channels.begin(); it != channels.end(); it++) {
        std::string chName = it->second->getName();
        if (toLowerCase(channelName) == toLowerCase(chName))
            return true;
    }
    return false;
}

bool ACommand::existentClient(std::string nickname) {
    std::map<int, Client*>::iterator it;
    std::map<int, Client*> clients = this->_server.getClients();

    for (it = clients.begin(); it != clients.end(); it++) {
        std::string clientNick = it->second->getNick();
        if (toLowerCase(clientNick) == toLowerCase(nickname))
            return true;
    }
    return false;
}

bool ACommand::findInQueue(std::queue<std::string> q, const std::string& target) {
    while (!q.empty()) {
        if (q.front() == target) {
            return true;  // Found the target string
        }
        q.pop();  // Move to the next element
    }
    return false;  // String not found
}

bool ACommand::existentClientOnChannel(const std::string &nickname, const std::string &channelName) {
    std::map<std::string, Channel *> channels = this->_server.getChannels();
    std::map<std::string, Channel *>::iterator it = channels.find(channelName);
    if (it == channels.end())
        return false;
    Channel *ch = it->second;
    std::map<int, Client *> clients = ch->getClients();
    for (std::map<int, Client *>::iterator clientIt = clients.begin(); clientIt != clients.end(); ++clientIt) {
        if (toLowerCase(clientIt->second->getNick()) == toLowerCase(nickname))
            return true;  // Found the client in the channel
    }
    std::map<int, Client *> operators = ch->getOperators();
    for (std::map<int, Client *>::iterator operatorIt = operators.begin(); operatorIt != operators.end(); ++operatorIt) {
        if (toLowerCase(operatorIt->second->getNick()) == toLowerCase(nickname))
            return true;  // Found the Operator in the channel
    }
    return false;  // User not found in the channel
}