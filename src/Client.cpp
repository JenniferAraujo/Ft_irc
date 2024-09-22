#include "Includes.hpp"

/*Client::Client(): _socketFD(0),
                    _regError(0),
                    _registration(0),
                    _ipAddr(""),
                    _hostname(""),
                    _password(""),
                    _nick(""),
                    _username(""),
                    _realname(""),
                    _lastActivityTime(0),
                    _connectTime(0),
                    _justJoined(false) {
                        Server a;
                        _server = a;
                    }*/

Client::Client(Server &server, time_t time)
    :   _regError(0),
        _registration(false),
        _server(server),
        _lastActivityTime(time),
        _connectTime(time),
        _justJoined(false) {}


Client::Client(const Client &cpy)
    :   _socketFD(cpy.getSocketFD()),
        _regError(cpy.getRegError()),
        _registration(cpy.getRegistration()),
        _ipAddr(cpy.getIpaddr()),
        _password(cpy.getPassword()),
        _nick(cpy.getNick()),
        _username(cpy.getUsername()),
        _realname(cpy.getRealname()),
        _server(cpy.getServer()),
        _justJoined(cpy.getJustJoined()) {}

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

ACommand* Client::createKick(std::istringstream &input){
    ACommand *command = new Kick(this->_server, *this);
    command->parsing(input);
    return command;
}

ACommand* Client::createPart(std::istringstream &input){
    ACommand *command = new Part(this->_server, *this);
    command->parsing(input);
    return command;
}

ACommand* Client::createInvite(std::istringstream &input){
    ACommand *command = new Invite(this->_server, *this);
    command->parsing(input);
    return command;
}

ACommand* Client::createTopic(std::istringstream &input){
    ACommand *command = new Topic(this->_server, *this);
    command->parsing(input);
    return command;
}

ACommand* Client::createPrivmsg(std::istringstream &input){
    ACommand *command = new Privmsg(this->_server, *this);
    command->parsing(input);
    return command;
}

ACommand* Client::createNotice(std::istringstream &input){
    ACommand *command = new Notice(this->_server, *this);
    command->parsing(input);
    return command;
}

ACommand* Client::createQuit(std::istringstream &input){
     std::cout << "Create cmd\n";
    ACommand *command = new Quit(this->_server, *this);
    command->parsing(input);
    return command;
}

std::queue<ACommand* >  Client::createCommand(std::vector<char> buf){
    std::cout << "Create cmd\n";
    std::string str(buf.begin(), buf.end());;
    std::istringstream input(str);
    std::string commands[] = {"CAP", "PASS", "NICK", "USER", "JOIN", "MODE", "WHO", "PING", "KICK", "PART", "INVITE", "TOPIC", "PRIVMSG", "NOTICE", "QUIT"};
    int N = static_cast<int>(ARRAY_SIZE(commands));
    ACommand* (Client::*p[])(std::istringstream&) = {&Client::createCap, &Client::createPass, &Client::createNick, &Client::createUser, &Client::createJoin, 
        &Client::createMode, &Client::createWho, &Client::createPing, &Client::createKick, &Client::createPart, &Client::createInvite, &Client::createTopic, 
            &Client::createPrivmsg, &Client::createNotice, &Client::createQuit};
    std::string cmd;
    std::string line;
    std::queue<ACommand *> result;
    while(std::getline(input, line)){
        trimChar(line, '\r');
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
    std::cout << formatServerMessage(BOLD_GREEN, "CLIENT", this->_socketFD, GREEN) << BOLD_GREEN << "Registration completed!" << RESET << std::endl;
    std::string msg;
    msg.append(RPL_WELCOME(this->_server.getHostname(), "Internet Fight Club", this->getNick(), this->getUsername(), this->getIpaddr()));
    msg.append(RPL_YOURHOST(this->_server.getHostname(), "rdjIRC", this->getNick(), "1.0"));
    msg.append(RPL_CREATED(this->_server.getHostname(), this->_server.getCreationTime(), this->getNick()));
    msg.append(RPL_MYINFO(this->_server.getHostname(), this->getNick(), "rdjIRC", "1.0"));
    msg.append(RPL_ISUPPORT(this->_server.getHostname(), this->getNick()));
    msg.append(RPL_MOTDSTART(this->_server.getHostname(), this->getNick(), "rdjIRC"));
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
    Message::sendMessage(this->_socketFD, msg, this->_server);
}

/* If the server is waiting to complete a lookup of client information (such as hostname or ident for a username),
there may be an arbitrary wait at some point during registration. Servers SHOULD set a reasonable timeout for these lookups. */
void    Client::registration(){
    if(this->getNick().empty()
                || this->getRealname().empty() ||this->getUsername().empty())
                    return ;
    this->setRegistration(true);
    if (this->_regError) {
        Message::sendMessage(this->_socketFD, ERR_PASSWDMISMATCH(this->_server.getHostname(), this->_nick), this->_server);
        this->_server.updateToRemove(this->_socketFD, "Connection Registration Failed");
        return ;
    }
    if (this->_password.empty()) {
        Message::sendMessage(this->_socketFD, ERR_UNKNOWNERROR(this->_server.getHostname(), this->_nick, "", "Missing password"), this->_server);
        this->_server.updateToRemove(this->_socketFD, "Connection Registration Failed");
        return ;
    }
    if(this->_server.findClient(this->_nick, this->_socketFD) != NULL
        && this->_server.findClient(this->_nick, this->_socketFD)->getRegistration()){
            Message::sendMessage(this->_socketFD, ERR_NICKNAMEINUSE(this->_server.getHostname(), "*", this->_nick), this->_server);
            this->_server.updateToRemove(this->_socketFD, "Connection Registration Failed");
            return ;
    }
    this->welcome();
}

void Client::verifyConnection(Server &server, const pollfd &pfd) {
    if (pfd.revents & POLLIN) {
        Client *client = new Client(server, time(NULL));
        struct sockaddr_in6 client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        memset(&client_addr, 0, sizeof(client_addr));
        try {
            client->_socketFD = accept(server.getSocketFD(), (struct sockaddr *)&client_addr, &client_addr_len);
            if (client->_socketFD == -1) {
                if (errno != EWOULDBLOCK && errno != EAGAIN){
                    delete client;
                    throw IRCException("[ERROR] Opening client socket went wrong");
                }
            }
            struct hostent *host = NULL;
            if (IN6_IS_ADDR_V4MAPPED(&client_addr.sin6_addr)) {
                struct in_addr ipv4_addr;
                memcpy(&ipv4_addr, &client_addr.sin6_addr.s6_addr[12], sizeof(ipv4_addr));
                host = gethostbyname(inet_ntoa(ipv4_addr));
                if (host == NULL)
                    throw IRCException("[ERROR] gethostbyname on client went wrong");
                char *temp = strdup(host->h_name);
                client->setIpAddr(temp);
                free(temp);
            }
        } catch(const std::exception &e) {
            std::cerr << RED << e.what() << RESET << std::endl;
        }

        std::cout << formatServerMessage(BOLD_GREEN, "CLIENT", client->_socketFD, GREEN) << "Connected from " << BOLD_GREEN << client->getIpaddr() << RESET << std::endl;

        server.updateNFDs(client->_socketFD);
        server.updateClients(client, client->_socketFD);
    }
}


Client::~Client() {}