#include "Includes.hpp"

Client::Client(Server &server)
    :   _authError(0),
        _registration(false),
        _server(server) {
}

ACommand* Client::createPass(std::istringstream &input){
    ACommand *command = new Pass(this->_server, *this);
    command->parsing(input);
    return command;
}

ACommand* Client::createNick(std::istringstream &input){
    ACommand *command = new Nick(this->_server, *this);
    command->parsing(input);
    return command;
}

ACommand* Client::createUser(std::istringstream &input){
    ACommand *command = new User(this->_server, *this);
    command->parsing(input);
    return command;
}

ACommand* Client::createJoin(std::istringstream &input){
    ACommand *command = new Join(this->_server, *this);
    command->parsing(input);
    return command;
}

//TODO cap who mode ping

ACommand*    Client::createCommand(std::vector<char> buf){
    std::string str(buf.begin(), buf.end());
    std::istringstream input(str);
    std::string commands[] = {"PASS", "NICK", "USER", "JOIN", "MODE", "WHO"}; //acrescentar commandos a medida que sao tratados
    ACommand* (Client::*p[])(std::istringstream&) = {&Client::createPass, &Client::createNick, &Client::createUser, &Client::createJoin};
    std::string cmd;
    std::getline(input, cmd, ' ');
    ACommand *command = NULL;
    for (int i = 0; i < 4; i++) {
        if(!commands[i].compare(cmd)) {
            command = (this->*p[i])(input);
            return command;
        }
    }
    return command;
}

// Função para verificar a conecção de clientes
// inet_ntop FUNÇÃO PROIBIDA, PROVAVELMENTE TEMOS DE MUDAR TUDO PARA O IPv4
void Client::verifyConnection(Server &server, const pollfd &pfd) {
    if (pfd.revents & POLLIN) {
        Client *client = new Client(server);

        struct sockaddr_in6 client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        memset(&client_addr, 0, sizeof(client_addr));

        client->_socketFD = accept(server.getSocketFD(), (struct sockaddr *)&client_addr, &client_addr_len);
        if (client->_socketFD == -1) {
            delete client;
            throw IRCException("[ERROR] Opening client socket went wrong");
        }

        char client_ip[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &(client_addr.sin6_addr), client_ip, INET6_ADDRSTRLEN);

        struct hostent *host = NULL;

        if (IN6_IS_ADDR_V4MAPPED(&client_addr.sin6_addr)) {
            struct in_addr ipv4_addr;
            memcpy(&ipv4_addr, &client_addr.sin6_addr.s6_addr[12], sizeof(ipv4_addr));

            host = gethostbyname(inet_ntoa(ipv4_addr));
        }

        std::cout << formatServerMessage(BOLD_GREEN, "CLIENT", 0) << "Client " << GREEN << "[" << client->_socketFD << "]" << RESET
                  << " connected from " << BOLD_CYAN << host->h_name << RESET << std::endl;
        client->setIpAddr(host->h_name);

        server.updateNFDs(client->_socketFD);
        server.updateClients(client, client->_socketFD);
    }
}



Client::~Client() {}