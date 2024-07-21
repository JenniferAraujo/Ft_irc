#include "includes/Client.hpp"

Client::Client() {

}

// Função para verificar a conecção de clientes
void Client::verifyConnection(Server server, std::vector<pollfd>::iterator it) {
    if (it->revents == POLLIN) {
        std::cout << "New Client connection incoming..." << std::endl;
        Client client;
        socklen_t addrlen = sizeof(client._socketInfo);
        client._socketFD = accept(server.getSocketFD(), reinterpret_cast<struct sockaddr *>(&client._socketInfo), &addrlen);
        if (client._socketFD == -1)
            throw IRCException("[ERROR] Opening client socket went wrong");
        server.updateNFDs(client._socketFD);
        server.updateClients(&client, client._socketFD);
        std::cout << "Client " << GREEN << "[" << client._socketFD << "]" << RESET << " connected!" << std::endl;
    }
}

Client::~Client() {}
