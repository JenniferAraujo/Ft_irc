#include "Includes.hpp"

Client::Client(Server &server)
    :   _authError(-1),
        _password(false), //initialized as false
        _authOver(false),
        _validCmd(false),
        _server(server) {
}

//*NOTA: nas mensagens enviadas pelo cliente a quebra de linha é representada por \r\n, por isso temos de ler até ao \r 
//e depois chamar outra vez a getline até ao fim da linha para terminar aquela linha

//Estas sao as funcoes auxiliares da autentificaçao: parsePassword, parseNick, parseUser

//Esta funçao verifica se a password com que o client tentou entrar é correta
//Se entrar nesta funçao é porque existe pass e a flag da password é ativada
//Se a pass for invalida guarda na variavel _authError o erro respetivo
void Client::parsePassword(std::istringstream &input){
    _password = true;
    std::string str;
    std::getline(input, str, '\r');
    if(_server.getPassword() != str)
        this->_authError = INVALIDPASS;
}

//Esta funcao só guarda o nick, precisa de validacoes futuras
//Se o nick for invalido deverá guardar na variavel _authError o erro respetivo = INVALIDNICK
void Client::parseNick(std::istringstream &input){
        std::getline(input, this->_nick, '\r');
}

//Esta funcao guarda o username e o realname, precisa de validacoes futuras
//Se o nick for invalido deverá guardar na variavel _authError o erro respetivo = INVALIDUSER
void Client::parseUser(std::istringstream &input){
    this->_authOver = true;
    std::string str;
    std::getline(input, this->_username, ' ');
    std::getline(input, str, ':');
    std::getline(input, this->_realname, '\r');
}


bool    Client::parseJoin(std::istringstream &input, std::string str){
    (void)str;
    std::string channel;
    std::getline(input, channel, '\r');
    channel.erase(0, 1);
    this->setCommand("JOIN");
    this->_fullCmd[this->getCommand()] = channel;
    return true;
}

//The parsing functions receive the full line of the command as an input stream, and the command as a string
//And store the command and its arguments on the correct variables
//These functions also need to perfoms validations on the arguments of the command
//And returns a bollean that sets a command as valid (true) or invalid (false)


//This function receives the input stream input
//getline reads each line from input and stores it into the string line
//We take the complete line (line) and create an input stream for getline to read (input_line)
//Then we read the first word of input_line and store it into the string cmd => the first word of a line is the command
//Now we compare the command with our string of commands, if it finds a valid command, the correct parsing function will be called
//If all tree commands were found or one of them is invalid, authentification is over
bool    Client::parseCap(std::istringstream &input, std::string str){
    std::string commands[] = {"PASS", "NICK", "USER"};
    void (Client::*p[])(std::istringstream&)= {&Client::parsePassword, &Client::parseNick, &Client::parseUser};
    std::string cmd;
    std::string line;
    int cmds_parsed = 0;
    while(std::getline(input, line)){
        std::istringstream  input_line(line);
        std::getline(input_line, cmd, ' ');
        for (int i = 0; i < 3; i++) {
            if(!commands[i].compare(cmd)){
                cmds_parsed++;
                (this->*p[i])(input_line);
            }
        }
    }
    //If all tree commands were found, one of them is invalid or there is no command PASS, authentification is over
    if(this->_authError != -1)
        this->_authOver = true;
    this->_command = str;
    //Se a autentificaçao acabou o commando é valido, ou seja, pode ser executado pode ser executado
    return this->_authOver;
}



//This function takes a buffer with a message sent from a client (the object that calls the function)
//It's goal is to parse the message, saving the commmand in the variable _command, so it can be executed by the server
//And saving the arguments of the command in this client atributes, so that they can be used by server when executing the command

//First we set the command as invalid, so we can check if the command receive will be valid or not
//The message, received as a char vector, is turned into a type string
//We take the full message (str) and create an input stream for getline to read (input)

//If the prior command is CAP and the authentification isn't done yet, then we continue the parsing of Cap aka the authentification process

//We have an array of commands, that will contain all commands our IRC treats
//We have an array of funtion pointers that correspond to each of the command's parsing
//Then we read the first word of input and store it into the string cmd => the first word of the msg is the command

//Now we compare the command with our string of commands, if it finds a valid command, the correct parsing function will be called
//The parsing functions return a boolean that sets the command as valid or invalid

void    Client::parseMessage(std::vector<char> buf){
    this->_validCmd = false;
    std::string str(buf.begin(), buf.end());
    std::istringstream input(str);
    if(this->_command == "CAP" && this->_authOver == false){
        this->_validCmd = this->parseCap(input, "CAP");
    }
    else {
        std::string commands[] = {"CAP", "JOIN"}; //acrescentar commandos a medida que sao tratados
        bool (Client::*p[])(std::istringstream&, std::string str) = {&Client::parseCap, &Client::parseJoin};
        std::string cmd;
        std::getline(input, cmd, ' ');
        for (int i = 0; i < 2; i++) {
            if(!commands[i].compare(cmd)) {
                this->_validCmd = (this->*p[i])(input, cmd);
            }
        }
    }
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

        std::cout << BOLD_PURPLE << "[CLIENT]\t" << RESET << "Client " << GREEN << "[" << client->_socketFD << "]" << RESET
                  << " connected from " << BOLD_CYAN << host->h_name << RESET << std::endl;
        client->setIpAddr(host->h_name);

        server.updateNFDs(client->_socketFD);
        server.updateClients(client, client->_socketFD);
    }
}



Client::~Client() {}