#include "includes/Client.hpp"

Client::Client() {}

/* void Client::parseCommand(std::istringstream input, std::string cmd){
        int N = 2;
        std::string commands[] = {"JOIN", "MSG"};
        void (Client::*p[])(std::istringstream&) = {&Client::parseJoin, &Client::parseMsg};
        this->_validCmd = false;
        for (int i = 0; i < N; i++) {
            if(!commands[i].compare(cmd))
                this->_validCmd = (this->*p[i])(input);
        }
} */

/* void    Client::saveInfo(std::istringstream input){

} */

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
    if(cmd == "CAP"){
        //this->saveInfo(input);
        this->_validCmd = false;
    }
    //else
        //this->parseCommand();
}

// Função para verificar a conecção de clientes
void    Client::verifyConnection(Server &server, const pollfd &pfd) {
    if (pfd.revents & POLLIN) {
        Client *client = new Client;
        client->_socketFD = accept(server.getSocketFD(), NULL, NULL);
        if (client->_socketFD == -1)
            throw IRCException("[ERROR] Opening client socket went wrong");
        server.updateNFDs(client->_socketFD);
        server.updateClients(client, client->_socketFD);
        std::cout << "Client " << GREEN << "[" << client->_socketFD << "]" << RESET << " connected!" << std::endl;
    }
}

Client::~Client() {}
