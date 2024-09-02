#include "Includes.hpp"

//This is typically only dispatched to clients that share a channel with the exiting user. 
//When the QUIT message is sent to clients, <source> represents the client that has exited the network.
//:dan-!d@localhost QUIT :Quit: Bye for now!
//                                 ; dan- is exiting the network with
//                                 the message: "Quit: Bye for now!"
void Server::removeClient(int fd, std::string reason){
    std::cout << "Removing Client with FD: " << fd << std::endl;
    if(this->_Clients.find(fd) != this->_Clients.end()){
        std::cout << "Found client: " << this->_Clients[fd]->getNick() << std::endl;
        for(std::map<std::string, Channel*>::iterator it = this->_Channels.begin(); it != this->_Channels.end(); ++it){
            //mandar uma quit msg para os canais onde o cliente se encontra
            //if(it->second->isClient(fd) || it->second->isOperator(fd))
                //manda msg para o canal
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

void Server::addInChannel(std::string channelName, std::string password, Client &client) {
    if (this->_Channels.find(channelName) != this->_Channels.end()) {
        if (!this->_Channels[channelName]->canJoin(client, password)) {
			// std::string msg = "Error: You cannot join the channel " + channelName + ".\r\n"; //NOTE - arrumar msg de erro
			// send(client.getSocketFD(), msg.c_str(), msg.length(), 0);
			return ;
    	}
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

// Channel     *Server::findChannel(std::string channel) {
//     for (std::map <std::string, Channel*>::iterator it = this->_Channels.begin(); it != _Channels.end(); ++it) {
//         if(it->second->getName() == channel)
//             return it->second;
//     }
//     return NULL;
// }
