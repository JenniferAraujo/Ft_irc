#include "includes/Server.hpp"

Server::Server() {
    std::cout << BOLD_WHITE << "Server" << RESET 
    << " default constructor called" << std::endl;
}

// Copy constructor
Server::Server(const Server& copy) {
    // Copy stuff
    std::cout << BOLD_WHITE << "Server" << RESET 
    << " copy constructor called" << std::endl;
}

// Copy assignment operator overload
Server& Server::operator= (const Server& copy) {
    // Self-assignment check
    if (this == &copy)
        return *this;
    // Copy stuff
    std::cout << BOLD_WHITE << "Server" << RESET 
    << " copy assignment operator overload" << std::endl;
    return *this;
}

Server::~Server() {
    std::cout << BOLD_WHITE << "Server" << RESET 
    << " destructor called" << std::endl;
}
