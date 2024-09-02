
#include "IRCException.hpp"

IRCException::IRCException(const std::string& msg): _msg(msg) {}

const char* IRCException::what() const throw(){
    return _msg.c_str();
}

IRCException::~IRCException() throw() {}