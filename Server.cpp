#include "Includes.hpp"

Server::Server()
{
    std::cout << BOLD_WHITE << "Server" << RESET
              << " default constructor called" << std::endl;
}

Server::Server(const int &port, const std::string &password) : _port(port),
                                                               _password(password)
{
    std::cout << BOLD_WHITE << "Server: " << RESET
              << "Port: " << _port
              << " | Pass: " << _password << std::endl;
}

Server::~Server() {}
