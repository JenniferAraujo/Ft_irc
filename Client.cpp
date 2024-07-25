#include "includes/Client.hpp"

Client::Client(Server &server): _passward(false), _server(server) {}

//Se a pass estiver errada invalid command
//O nick e o user só imprimem com \n e eu n percebo porque
//VALIDAÇOES - ler documentacao
bool    Client::parsePassword(std::istringstream &input, std::string str){
        std::string in;
        std::getline(input, str, '\r');
        if(_server.getPassward() != str){
            _passward = false;
            return false;
        }
        _passward = true;
        return false;
}

bool    Client::parseNick(std::istringstream &input, std::string str){
        this->_command = str;
        std::getline(input, this->_nick, '\r');
        std::getline(input, str); //pass \n
        return true;
}

bool    Client::parseUser(std::istringstream &input, std::string str){

        std::getline(input, this->_username, ' ');
        std::getline(input, str, ':');
        std::getline(input, this->_realname, '\r');
        std::getline(input, str); //pass \n
        return true;

}

//Client info
//JOIN
//MSG
//Neste momento nao faço nada se o comando for invalido
//Se for para guardar as informaçoes guardo a bool validCmd = false
void    Client::parseMessage(std::vector<char> buf){
    std::string str(buf.begin(), buf.end());
    std::string commands[] = {"PASS", "NICK", "USER"};
    bool (Client::*p[])(std::istringstream&, std::string str) = {&Client::parsePassword, &Client::parseNick, &Client::parseUser};
    std::istringstream input(str);
    std::string cmd;
    std::string line;
    while(std::getline(input, line)){
        std::istringstream  input_line(line);
        std::getline(input_line, cmd, ' ');
        for (int i = 0; i < 3; i++) {
            if(!commands[i].compare(cmd)){
                this->_validCmd = (this->*p[i])(input_line, cmd);
            }
        }
    }
}

// Função para verificar a conecção de clientes
void    Client::verifyConnection(Server &server, const pollfd &pfd) {
    if (pfd.revents & POLLIN) {
        Client *client = new Client(server);
        client->_server = server;
        client->_socketFD = accept(server.getSocketFD(), NULL, NULL);
        if (client->_socketFD == -1)
            throw IRCException("[ERROR] Opening client socket went wrong");
        server.updateNFDs(client->_socketFD);
        server.updateClients(client, client->_socketFD);
        std::cout << "Client " << GREEN << "[" << client->_socketFD << "]" << RESET << " connected!" << std::endl;
    }
}

Client::~Client() {}