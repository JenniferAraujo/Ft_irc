#include "Includes.hpp"

void Server::removeClient(int fd, std::string reason){
    if(this->_Clients.find(fd) != this->_Clients.end()){
        std::cout << formatServerMessage(BOLD_RED, "CLIENT", fd, RED) << "Disconnecting " << RED << "[" << reason << "]" << RESET <<std::endl;
        for(std::map<std::string, Channel*>::iterator it = this->_Channels.begin(); it != this->_Channels.end(); ++it){
            if(it->second->isClient(fd) || it->second->isOperator(fd))
                it->second->sendMessage(QUIT(this->_Clients[fd]->getNick(), this->_Clients[fd]->getUsername(), this->_Clients[fd]->getIpaddr(), reason), fd);
            it->second->removeOperator(fd);
            it->second->removeClient(fd);
            it->second->removeInvited(fd);
        }
        if (!reason.empty())
            Message::sendMessage(fd, ERROR(reason), *this);
        close(fd);
        delete this->_Clients[fd];
        this->_Clients.erase(fd);
    }
    for (std::vector<pollfd>::iterator it = this->_NFDs.begin(); it != this->_NFDs.end(); ++it) {
        if (it->fd == fd) {
            this->_NFDs.erase(it);
            break;
        }
    }
}

void Server::getServerInfo() {
    char hostbuffer[256];
    struct hostent *host_entry;
    int hostname;

    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    if (hostname == -1) {
        throw IRCException("[ERROR] gethostname went wrong");
    }

    host_entry = gethostbyname(hostbuffer);
    if (host_entry == NULL) {
        throw IRCException("[ERROR] gethostbyname went wrong");
    }

    char *IPbuffer = inet_ntoa(*(struct in_addr*) host_entry->h_addr_list[0]);
    if (IPbuffer == NULL) {
        throw IRCException("[ERROR] inet_ntoa went wrong");
    }
    char *temp = strdup(host_entry->h_name);
    this->_hostName = temp;
    free(temp);
    this->_hostIP = IPbuffer;
}

int Server::addInChannel(std::string channelName, std::string password, Client &client) {
    int aux;
    if (this->_Channels.find(channelName) != this->_Channels.end()) {
        if ((aux = this->_Channels[channelName]->canJoin(client, password)) != 0)
			return aux;
		this->_Channels[channelName]->addClient(client);
        std::cout << formatServerMessage(BOLD_YELLOW, "JOINED", 0, "") << GREEN << "[" << client.getNick() << "]" << RESET << " entered Channel " << BOLD_YELLOW << channelName << RESET << std::endl;
        this->printChannelInfo(channelName);
    }
    else {
        Channel *channel = new Channel(channelName);
        this->_Channels[channelName] = channel;
        this->_Channels[channelName]->addOperator(client.getSocketFD(), &client);
        std::cout << formatServerMessage(BOLD_YELLOW, "JOINED", 0, "") << GREEN << "[" << client.getNick() << "]" << RESET << " created Channel " << BOLD_YELLOW << channelName << RESET << std::endl;
        this->printChannelInfo(channelName);
    }
    return 0;
}

void    Server::printChannelInfo(std::string channelName) {
    std::cout << BOLD_YELLOW << "[CHANNEL INFO]\t\t" << RESET << "@[";
    showMap(this->_Channels[channelName]->getOperators());
    std::cout << "] [";
    showMap(this->_Channels[channelName]->getClients());
    std::cout << "] I[";
    std::vector<int> fds = this->_Channels[channelName]->getInvited();
    if (fds.empty())
        std::cout << "-";
    for (std::vector<int>::iterator it = fds.begin(); it != fds.end(); ++it) {
        Client*     c = this->getClients()[*it];
        std::cout << c->getNick();
        if (it + 1 != fds.end())
            std::cout << ", ";
    }
    std::cout << "] l[";
    if (this->_Channels[channelName]->getUserLimit() == -1)
        std::cout << "-";
    else
        std::cout << this->_Channels[channelName]->getUserLimit();
    std::cout << "] k[" << (this->_Channels[channelName]->getPassword().empty() ? "-" : this->_Channels[channelName]->getPassword());
    std::cout << "] i[";
    if (!this->_Channels[channelName]->isInviteOnly())
        std::cout << "-";
    else
        std::cout << "X";
    std::cout << "] t[";
    if (!this->_Channels[channelName]->isTopicLocked())
        std::cout << "-";
    else
        std::cout << "X";
    std::cout << "]" << std::endl;
}

Client    *Server::findClient(std::string nick, int skipFd){
    for (std::map <int, Client *>::iterator it = this->_Clients.begin(); it != _Clients.end(); ++it) {
        if(it->second->getSocketFD() != skipFd && it->second->getNick() == nick)
            return it->second;
    }
    return NULL;
}

int     Server::getClientByNick(std::string nick) {
    for (std::map <int, Client *>::iterator it = this->_Clients.begin(); it != _Clients.end(); ++it) {
        if(it->second->getNick() == nick)
            return it->second->getSocketFD();
    }
    return -1;
}

void    Server::display() const {
    std::cout << formatServerMessage(BOLD_CYAN, "SERVER", 0, "") << "Socket with fd " << CYAN "[" << this->_socketFD << "]" << RESET
              << " bound on port " << CYAN << this->_port << RESET << std::endl;
    std::cout << formatServerMessage(BOLD_CYAN, "SERVER", 0, "") << "Server listening only " << CYAN << 10 << RESET << " connections"
              << std::endl;
    std::cout << formatServerMessage(BOLD_CYAN, "SERVER", 0, "") << "Hostname: " << this->_hostName << std::endl;
    std::cout << formatServerMessage(BOLD_CYAN, "SERVER", 0, "") << "Host IP: " << this->_hostIP << "\n" << std::endl;

}

void Server::stopCompilation(int signal) {
    std::cout << "Compilation stopped due to signal: " << signal << std::endl;
    for(std::map<std::string, Channel*>::iterator it = this->_Channels.begin(); it != this->_Channels.end(); ++it){
            delete it->second;
    }
    this->_Channels.clear();
    for(std::map<int, Client*>::iterator it = this->_Clients.begin(); it != this->_Clients.end(); ++it){
            delete it->second;
    }
    this->_Clients.clear();
    std::exit(0);
}

void Server::signals() {
    signal(SIGINT, signalHandler);
}