#include "includes/Client.hpp"

Client::Client(Server &server): _server(server){}

//Se a pass estiver errada invalid command
//O nick e o user só imprimem com \n e eu n percebo porque
//VALIDAÇOES - ler documentacao
bool    Client::parseClient(std::istringstream &input, std::string str){
        this->_command = str;
        std::string in;
        std::getline(input, str); //passa a 1a linha
        std::getline(input, str, ' '); //linha da pass
        if(str == "PASS"){
            std::getline(input, str, '\r');
            if(_server.getPassward() != str){
                _passward = false;
                return false;
            }       
        }
        else{
            _passward = false;
            return false;
        }
        _passward = true;
        std::getline(input, str); //pass \n
        std::getline(input, str, ' ');
        if(str == "NICK"){
            std::getline(input, this->_nick, '\r'); 
            std::getline(input, str); //pass \n
        }
        std::getline(input, str, ' ');
        if(str == "USER"){
            std::getline(input, this->_username, ' ');
            std::getline(input, str, ':');
            std::getline(input, this->_realname, '\r');
            std::getline(input, str); //pass \n
        }
        return true;
}

//Client info
//JOIN
//MSG
//Neste momento nao faço nada se o comando for invalido
//Se for para guardar as informaçoes guardo a bool validCmd = false
void    Client::parseMessage(std::vector<char> buf){
    std::string str(buf.begin(), buf.end());
    std::istringstream input(str);
    std::string cmd;
    std::getline(input, cmd, ' ');
    std::string commands[] = {"CAP"};
    bool (Client::*p[])(std::istringstream&, std::string str) = {&Client::parseClient};
    for (int i = 0; i < 1; i++) {
        if(!commands[i].compare(cmd)){
            this->_validCmd = (this->*p[i])(input, cmd);
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
