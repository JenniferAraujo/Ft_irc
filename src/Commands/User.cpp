#include "Includes.hpp"

User::User(Server& server, Client& client): ACommand("USER", server, client) {};

void User::parsing(std::istringstream &input){
    std::string str;
    std::getline(input, this->_username, ' ');
    if(this->_username.empty())
        _error = NEEDMOREPARAMS; //USER
    if(this->_username.length() > USERLEN - 1)
        this->_username = this->_username.substr(0, USERLEN -1); //usernamemuitolongo => usernamemui
    this->_username.insert(this->_username.begin(), '~'); //username => ~username
    std::getline(input, str, ':');
    if(std::cin.eof() || str[0] != '0' || str[2] != '*' || str[3] != ' '){
        _error = NEEDMOREPARAMS; //nao tem a formataçao "0 * :"
        return ;
    }
    std::getline(input, this->_realname);
    if(this->_realname.empty())
        _error = NEEDMOREPARAMS; //USER 0 * :
    this->trimChar(this->_realname, '\r');
}

void User::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0, "") << RESET << this->_name << std::endl;
    std::string msg;
    if(this->_client.getRegistration())
        Message::sendMessage(this->_client.getSocketFD(), ERR_ALREADYREGISTERED(this->_server.getHostname(), this->_client.getNick()), this->_server);
    else if (this->_error == NEEDMOREPARAMS)
        Message::sendMessage(this->_client.getSocketFD(), ERR_NEEDMOREPARAMS(this->_server.getHostname(), this->_client.getNick(), this->_name), this->_server);
    else{
        this->_client.setUsername(this->_username);
        this->_client.setRealname(this->_realname);
    }
}

void User::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << " | Name: " << this->_username << " | Real name: " << this->_realname << std::endl;
}