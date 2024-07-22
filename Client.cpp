#include "includes/Client.hpp"

Client::Client(sockaddr_in6 socketInfo): _socketInfo(socketInfo) {}

void Client::getInfo() {
    std::vector<char> buf(5000);
    int bytes = 1;
    while (bytes > 0) {
        bytes = recv(this->_socketFD, buf.data(), buf.size(), 0);
        std::cout << "N Bytes received:" << bytes << std::endl; 
        std::cout << buf.data() << "." << std::endl;
    }
}

// Função para verificar a conecção de clientes
void Client::verifyConnection(Server server, std::vector<pollfd>::iterator it) {
    if (it->revents == POLLIN) {
        std::cout << "New Client connection incoming..." << std::endl;
        Client client(server.getSocketInfo());
        socklen_t addrlen = sizeof(client._socketInfo);
        client._socketFD = accept(server.getSocketFD(), reinterpret_cast<struct sockaddr *>(&client._socketInfo), &addrlen);
        if (client._socketFD == -1)
            throw IRCException("[ERROR] Opening client socket went wrong");
        client.getInfo();
        server.updateNFDs(client._socketFD);
        server.updateClients(&client, client._socketFD);
        std::cout << "Client " << GREEN << "[" << client._socketFD << "]" << RESET << " connected!" << std::endl;
    }
}

Client::~Client() {}
