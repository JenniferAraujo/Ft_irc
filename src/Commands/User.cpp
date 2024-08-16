#include "Includes.hpp"

User::User(Server& server, Client& client): ACommand("USER", server, client) {};

//Esta funcao guarda o username e o realname, precisa de validacoes futuras
//Se o nick for invalido deverá guardar na variavel _authError o erro respetivo = INVALIDUSER
//Users nao podem ser repetidos??
//username e realname sao campos obrigatorios
void User::parsing(std::istringstream &input){
    std::string str;
    std::getline(input, this->_username, ' ');
    std::getline(input, str, ':');
    std::getline(input, this->_realname, '\n');
    this->trimChar(this->_realname, '\r');
}

void User::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << RESET << this->_name << std::endl;
    std::string msg;
    if(this->_client.getRegistration()){
        msg.append(ERROR("You may not reregister"));
        send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
        return ;
    }
    if (this->_error) {
        msg.append(ERROR("userword incorrect"));
        send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
        //this->_server._toRemove.push_back(this->_client.getSocketFD()); //TODO condiçao para nao haver repetidos (funçao para adicionar ao toRemove)
        this->_client.setRegError(INVALIDUSER);
    }
    else{
        this->_client.setUsername(this->_username);
        this->_client.setRealname(this->_realname);
    }
}

void User::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << " | Name: " << this->_username << " | Real name: " << this->_realname << std::endl;
}