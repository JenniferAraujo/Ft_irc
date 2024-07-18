#include "includes/Exception.hpp"

Exception::Exception() {
    std::cout << BOLD_WHITE << "Exception" << RESET 
    << " default constructor called" << std::endl;
}

// Copy constructor
Exception::Exception(const Exception& copy) {
    // Copy stuff
    std::cout << BOLD_WHITE << "Exception" << RESET 
    << " copy constructor called" << std::endl;
}

// Copy assignment operator overload
Exception& Exception::operator= (const Exception& copy) {
    // Self-assignment check
    if (this == &copy)
        return *this;
    // Copy stuff
    std::cout << BOLD_WHITE << "Exception" << RESET 
    << " copy assignment operator overload" << std::endl;
    return *this;
}

Exception::~Exception() {
    std::cout << BOLD_WHITE << "Exception" << RESET 
    << " destructor called" << std::endl;
}
