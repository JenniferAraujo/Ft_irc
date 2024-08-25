#include "Includes.hpp"

User::User(Server& server, Client& client): ACommand("USER", server, client) {};

void User::parsing(std::istringstream &input){
    std::string str;
    std::getline(input, this->_username, ' ');
    if(this->_username.empty())
        _error = NEEDMOREPARAMS; //USER
    if(this->_username.length() > USERLEN - 1)
        this->_username = this->_username.substr(0, USERLEN -1);
    this->_username.insert(this->_username.begin(), '~');
    std::getline(input, str, ':');
    if(std::cin.eof() || str[0] != '0' || str[2] != '*'){
        _error = NEEDMOREPARAMS;
        return ;
    }
    std::getline(input, this->_realname);
    if(this->_realname.empty())
        _error = NEEDMOREPARAMS;
    this->trimChar(this->_realname, '\r');
}

void User::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << RESET << this->_name << std::endl;
    std::string msg;
    if(this->_client.getRegistration()){
        msg.append(ERR_ALREADYREGISTERED(this->_server.getHostname(), this->_client.getNick()));
        send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
        return ;
    }
    if (this->_error == NEEDMOREPARAMS){
        if (this->_client.getNick().empty())
            msg.append(ERR_NEEDMOREPARAMS(this->_server.getHostname(), "*", this->_name));
        else
            msg.append(ERR_NEEDMOREPARAMS(this->_server.getHostname(), this->_client.getNick(), this->_name));
        send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
        return ;
    }
    this->_client.setUsername(this->_username);
    this->_client.setRealname(this->_realname);
}

void User::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << " | Name: " << this->_username << " | Real name: " << this->_realname << std::endl;
}