#include "Includes.hpp"

void Server::removeClient(int fd){
    if(this->_Clients.find(fd) != this->_Clients.end()){
        close(fd);
        this->_Clients.erase(fd);
    }
}

// ISTO ESTÁ ERRADO PK NÃO SE PODE USAR A GETHOSTNAME, MAS SIM A GETHOSTBYNAME
void Server::getServerInfo() {
    char hostbuffer[256];
    char *IPbuffer;
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

    IPbuffer = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0]));

    this->_hostName = host_entry->h_name;
    this->_hostIP = IPbuffer;
    std::cout << BOLD_YELLOW << "[SERVER INFO]\t" << RESET << "Hostname: " << host_entry->h_name << std::endl;
    std::cout << BOLD_YELLOW << "[SERVER INFO]\t" << RESET << "Host IP: " << IPbuffer << "\n" << std::endl;
}