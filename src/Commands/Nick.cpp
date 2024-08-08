#include "Includes.hpp"

Nick::Nick(const Server& server, const Client& client): ACommand("NICK", server, client) {};

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