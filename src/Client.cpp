#include "Includes.hpp"

Client::Client(Server &server)
    :   _regError(0),
        _registration(false),
        _server(server) {}


Client::Client(const Client &cpy)
    :   _socketFD(cpy.getSocketFD()),
        _regError(cpy.getRegError()),
        _registration(cpy.getRegistration()),
        _ipAddr(cpy.getIpaddr()),
        _password(cpy.getPassword()),
        _nick(cpy.getNick()),
        _username(cpy.getUsername()),
        _realname(cpy.getRealname()),
        _server(cpy.getServer()) {}

ACommand* Client::createCap(std::istringstream &input) {
    ACommand *command = new Cap(this->_server, *this);
    command->parsing(input);
    return command;
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

ACommand* Client::createMode(std::istringstream &input){
    ACommand *command = new Mode(this->_server, *this);
    command->parsing(input);
    return command;
}

ACommand* Client::createWho(std::istringstream &input){
    ACommand *command = new Who(this->_server, *this);
    command->parsing(input);
    return command;
}

ACommand* Client::createPing(std::istringstream &input){
    ACommand *command = new Ping(this->_server, *this);
    command->parsing(input);
    return command;
}


std::queue<ACommand* >  Client::createCommand(std::vector<char> buf){
    std::string str(buf.begin(), buf.end());
    //std::cout << "BUF: " << str << std::endl;
    std::istringstream input(str);
    std::string commands[] = {"CAP", "PASS", "NICK", "USER", "JOIN", "MODE", "WHO", "PING"};
    int N = static_cast<int>(ARRAY_SIZE(commands));
    ACommand* (Client::*p[])(std::istringstream&) = {&Client::createCap, &Client::createPass, &Client::createNick, &Client::createUser, &Client::createJoin, &Client::createMode, &Client::createWho, &Client::createPing};
    std::string cmd;
    std::string line;
    std::queue<ACommand *> result;
    while(std::getline(input, line)){
        std::istringstream  input_line(line);
        std::getline(input_line, cmd, ' ');
        for (int i = 0; i < N; i++) {
            if(!commands[i].compare(cmd)) {
                ACommand *command = (this->*p[i])(input_line);
                result.push(command);
            }
        }
    }
    return result;
}

void Client::welcome() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << RESET << "WELCOME" << std::endl;
    std::string msg;
    msg.append(RPL_WELCOME(this->_server.getHostname(), "Internet Fight Club", this->getNick(), this->getUsername(), this->getIpaddr()));
    msg.append(RPL_YOURHOST(this->_server.getHostname(), "servername", this->getNick(), "version"));
    msg.append(RPL_CREATED(this->_server.getHostname(), this->_server.getCreationTime(), this->getNick()));
    msg.append(RPL_MYINFO(this->_server.getHostname(), this->getNick(), "servername"));
    msg.append(RPL_ISUPPORT(this->_server.getHostname(), this->getNick()));
    msg.append(RPL_MOTDSTART(this->_server.getHostname(), this->getNick(), "servername"));
    msg.append(RPL_MOTD(this->_server.getHostname(), "  ________________", this->getNick()));
    msg.append(RPL_MOTD(this->_server.getHostname(), " /______________ /|", this->getNick()));
    msg.append(RPL_MOTD(this->_server.getHostname(), "|  ___________  | |", this->getNick()));
    msg.append(RPL_MOTD(this->_server.getHostname(), "| |           | | |", this->getNick()));
    msg.append(RPL_MOTD(this->_server.getHostname(), "| |  ft_irc   | | |", this->getNick()));
    msg.append(RPL_MOTD(this->_server.getHostname(), "| |    by:    | | |", this->getNick()));
    msg.append(RPL_MOTD(this->_server.getHostname(), "| |           | | |", this->getNick()));
    msg.append(RPL_MOTD(this->_server.getHostname(), "| |  r, j, d  | | |", this->getNick()));
    msg.append(RPL_MOTD(this->_server.getHostname(), "| |___________| | |  ___", this->getNick()));
    msg.append(RPL_MOTD(this->_server.getHostname(), "|_______________|/  /  /", this->getNick()));
    msg.append(RPL_MOTD(this->_server.getHostname(), "                   /__/", this->getNick()));
    msg.append(RPL_ENDOFMOTD(this->_server.getHostname(), this->getNick()));
    send(this->getSocketFD(), msg.c_str(), msg.length(), 0);
}

/* If the server is waiting to complete a lookup of client information (such as hostname or ident for a username), 
there may be an arbitrary wait at some point during registration. Servers SHOULD set a reasonable timeout for these lookups. */
void    Client::registration(){
    if(this->getNick().empty()
                || this->getRealname().empty() ||this->getUsername().empty())
                    return ;
    std::string msg;
    this->setRegistration(true);
    if (this->_password.empty()) {
        msg = ERR_UNKNOWNERROR(this->_server.getHostname(), this->_nick, "", "Missing password");
        send(this->getSocketFD(), msg.c_str(), msg.length(), 0);
        //this->_toRemove.push_back(this->getSocketFD()); //TODO create function Server::addToRemove
        this->_regError = UNKNOWNERROR;
        return ;
    }
    if (this->_regError == PASSWDMISMATCH) {
        msg = ERR_PASSWDMISMATCH(this->_server.getHostname(), this->_nick);
        send(this->getSocketFD(), msg.c_str(), msg.length(), 0);
        //this->_toRemove.push_back(this->getSocketFD());
        return ;
    }
    this->welcome();
    //TODO disconnection depending on timeout
    /*if(this->_ping > 5){
        this->_toRemove.push_back(this->getSocketFD());
    }*/
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

        // struct hostent *host = NULL;
        // if (IN6_IS_ADDR_V4MAPPED(&client_addr.sin6_addr)) {
        //     struct in_addr ipv4_addr;
        //     memcpy(&ipv4_addr, &client_addr.sin6_addr.s6_addr[12], sizeof(ipv4_addr));
        //     host = gethostbyname(inet_ntoa(ipv4_addr));
        // }

        std::cout << formatServerMessage(BOLD_GREEN, "CLIENT", 0) << "Client " << GREEN << "[" << client->_socketFD << "]" << RESET
                  << " connected from " << BOLD_CYAN << client_ip << RESET << std::endl << std::endl;
        client->setIpAddr(client_ip);

        server.updateNFDs(client->_socketFD);
        server.updateClients(client, client->_socketFD);
    }
}



Client::~Client() {}