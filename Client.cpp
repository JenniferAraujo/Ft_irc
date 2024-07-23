#include "includes/Client.hpp"

Client::Client(sockaddr_in6 socketInfo): _socketInfo(socketInfo) {}

void Client::parseInfo(std::vector<char> buf){
    (void)buf;
}

// Função para verificar a conecção de clientes
void Client::verifyConnection(Server &server, std::vector<pollfd>::iterator it) {
    if (it->revents & POLLIN) {
        Client *client = new Client(server.getSocketInfo());
        socklen_t addrlen = sizeof(client->_socketInfo);
        client->_socketFD = accept(server.getSocketFD(), reinterpret_cast<struct sockaddr *>(&client->_socketInfo), &addrlen);
        if (client->_socketFD == -1)
            throw IRCException("[ERROR] Opening client socket went wrong");
        server.updateNFDs(client->_socketFD);
        server.updateClients(client, client->_socketFD);
        std::cout << "Client " << GREEN << "[" << client->_socketFD << "]" << RESET << " connected!" << std::endl;
    }
}

Client::~Client() {}
