#include "includes/Client.hpp"

Client::Client() {}

// Função para verificar a conecção de clientes
void Client::verifyConnection(Server server) {
    std::cout << "verifyConnection" << std::endl;
    (void)server;
}

Client::~Client() {}
