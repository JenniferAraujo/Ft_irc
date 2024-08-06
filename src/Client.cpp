#include "Includes.hpp"

Client::Client(Server &server)
    :   _authError(0),
        _registration(false),
        _server(server) {
}

//Esta funcao só guarda o nick, precisa de validacoes futuras
//Se o nick for invalido deverá guardar na variavel _authError o erro respetivo = INVALIDNICK
//Info do chat gpt: estudar protocolos e subject
//!O nickname deve ser unico
    //Entre 1 a 9 caracteres
    //Pode incluir letras (A-Z, a-z), dígitos (0-9) e alguns caracteres especiais (-, _, \, [, ], ^, {, }, |).
    //Deve começar com uma letra.
void Client::parseNick(std::istringstream &input){
        std::getline(input, this->_nick, '\r');
}

//Esta funcao guarda o username e o realname, precisa de validacoes futuras
//Se o nick for invalido deverá guardar na variavel _authError o erro respetivo = INVALIDUSER
//Users nao podem ser repetidos??
//username e realname sao campos obrigatorios
void Client::parseUser(std::istringstream &input){
    this->_registration = true;
    std::string str;
    std::getline(input, this->_username, ' ');
    std::getline(input, str, ':');
    std::getline(input, this->_realname, '\r');
}

//The parsing functions receive the full line of the command as an input stream, and the command as a string
//And store the command and its arguments on the correct variables
//These functions also need to perfoms validations on the arguments of the command
//And returns a bollean that sets a command as valid (true) or invalid (false)





bool Client::parseMode(std::istringstream &input, std::string str){
        (void)str;
        //std::cout << "Parse Mode" << std::endl;
        //std::cout << "\t" << str << std::endl;
        std::string channel;
        std::getline(input, channel, '\r');
        channel.erase(0, 1);
        this->setCommand("MODE");
        this->_fullCmd[this->getCommand()] = channel;
        return true;
}

bool Client::parseWho(std::istringstream &input, std::string str){
       (void)str;
        //std::cout << "Parse Who" << std::endl;
        //std::cout << "\t" << str << std::endl;
        std::string channel;
        std::getline(input, channel, '\r');
        channel.erase(0, 1);
        this->setCommand("WHO");
        this->_fullCmd[this->getCommand()] = channel;
        return true;
}

ACommand* Client::createPass(std::istringstream &input){
    ACommand *command = new Pass(this->_server, *this);
    command->parsing(input);
    return command;
}

ACommand* Client::createJoin(std::istringstream &input){
    ACommand *command = new Join(this->_server, *this);
    command->parsing(input);
    return command;
}

ACommand* Client::createWho(std::istringstream &input){
    ACommand *command = new Pass(this->_server, *this);
    command->parsing(input);
    return command;
}

ACommand* Client::createMode(std::istringstream &input){
    ACommand *command = new Pass(this->_server, *this);
    command->parsing(input);
    return command;
}

ACommand*    Client::createCommand(std::vector<char> buf){
    std::string str(buf.begin(), buf.end());
    std::istringstream input(str);
    std::string commands[] = {"PASS", "NICK", "USER", "JOIN", "MODE", "WHO"}; //acrescentar commandos a medida que sao tratados
    bool (Client::*p[])(std::istringstream&, std::string str) = {&Client::createPass, &Client::createNick, 
        &Client::createUser, &Client::createJoin, &Client::parseMode, &Client::parseWho};
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