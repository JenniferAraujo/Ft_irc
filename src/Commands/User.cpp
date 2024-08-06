#include "Includes.hpp"

User::User(const Server& server, const Client& client): ACommand("USER", server, client) {};

//Esta funcao guarda o username e o realname, precisa de validacoes futuras
//Se o nick for invalido deverá guardar na variavel _authError o erro respetivo = INVALIDUSER
//Users nao podem ser repetidos??
//username e realname sao campos obrigatorios
void User::parsing(std::istringstream &input){
    std::string str;
    std::getline(input, this->_name, ' ');
    std::getline(input, str, ':');
    std::getline(input, this->_realname, '\r');
}

User::~User() {}