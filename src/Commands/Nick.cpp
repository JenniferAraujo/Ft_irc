#include "Includes.hpp"

Nick::Nick(Server& server, Client& client): ACommand("NICK", server, client) {};

//Esta funcao só guarda o nick, precisa de validacoes futuras
//Se o nick for invalido deverá guardar na variavel _authError o erro respetivo = INVALIDNICK
//Info do chat gpt: estudar protocolos e subject
//!O nickname deve ser unico
    //Entre 1 a 9 caracteres
    //Pode incluir letras (A-Z, a-z), dígitos (0-9) e alguns caracteres especiais (-, _, \, [, ], ^, {, }, |).
    //Deve começar com uma letra.
void Nick::parsing(std::istringstream &input){
    std::getline(input, this->_nick, '\n');
    this->trimChar(this->_nick, '\r');
}

void Nick::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << RESET << this->_name << std::endl;
    std::string msg;
    if (this->_error) {
        msg.append(ERROR("Invalid Nick"));
        send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
        //this->_toRemove.push_back(client.getSocketFD()); //TODO create function Server::addToRemove
        this->_client.setRegError(INVALIDNICK);
    }
    else
        this->_client.setNick(this->_nick);
}

void Nick::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << " | Nick: " << this->_nick << std::endl;
}