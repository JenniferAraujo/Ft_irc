#include "includes/Client.hpp"

Client::Client() {
    std::cout << BOLD_WHITE << "Client" << RESET 
    << " default constructor called" << std::endl;
}

// Copy constructor
Client::Client(const Client& copy) {
    // Copy stuff
    std::cout << BOLD_WHITE << "Client" << RESET 
    << " copy constructor called" << std::endl;
}

// Copy assignment operator overload
Client& Client::operator= (const Client& copy) {
    // Self-assignment check
    if (this == &copy)
        return *this;
    // Copy stuff
    std::cout << BOLD_WHITE << "Client" << RESET 
    << " copy assignment operator overload" << std::endl;
    return *this;
}

Client::~Client() {
    std::cout << BOLD_WHITE << "Client" << RESET 
    << " destructor called" << std::endl;
}
