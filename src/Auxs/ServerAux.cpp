#include "Includes.hpp"

void Server::removeClient(int fd, std::string error){
    std::cout << "Removing Client with FD: " << fd << std::endl;
    if(this->_Clients.find(fd) != this->_Clients.end()){
        std::cout << "Found client: " << this->_Clients[fd]->getNick() << std::endl;
        std::string msg = ERROR(error);
        send(this->_Clients[fd]->getSocketFD(), msg.c_str(), msg.length(), 0);
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

// FIXME - ISTO ESTÁ ERRADO PK NÃO SE PODE USAR A GETHOSTNAME, MAS SIM A GETHOSTBYNAME
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

    std::cout << BOLD_YELLOW << "[SERVER INFO]\t" << RESET << "Hostname: " << this->_hostName << std::endl;
    std::cout << BOLD_YELLOW << "[SERVER INFO]\t" << RESET << "Host IP: " << this->_hostIP << "\n" << std::endl;
}

void Server::addInChannel(std::string channelName, Client &client) {
    if (this->_Channels.find(channelName) != this->_Channels.end()) {
        std::cout << formatServerMessage(BOLD_YELLOW, "JOINED", 0) << client.getNick() << " entered the channel " << BOLD_YELLOW << channelName << RESET << std::endl;
        this->_Channels[channelName]->addClient(client);
    }
    else {
        std::cout << formatServerMessage(BOLD_YELLOW, "JOINED", 0) << client.getNick() << " created the channel " << BOLD_YELLOW << channelName << RESET << std::endl;
        Channel *channel = new Channel(channelName);
        this->_Channels[channelName] = channel;
        this->_Channels[channelName]->addClient(client);
    }
}

Client    *Server::findClient(std::string nick){
    for (std::map <int, Client *>::iterator it = this->_Clients.begin(); it != _Clients.end(); ++it) {
        if(it->second->getNick() == nick)
            return it->second;
    }
    return NULL;
}