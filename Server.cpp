#include "includes/Server.hpp"

Server::Server() {
    std::cout << BOLD_WHITE << "Server" << RESET 
    << " default constructor called" << std::endl;
}

Server::~Server() {
    std::cout << BOLD_WHITE << "Server" << RESET 
    << " destructor called" << std::endl;
}
