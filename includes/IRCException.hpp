#ifndef IRCEXCEPTION_HPP
# define IRCEXCEPTION_HPP

#include "Includes.hpp"

class IRCException: public std::exception {
    public:
        explicit IRCException(const std::string& msg);
        virtual const char* what() const throw();
        virtual ~IRCException() throw();
    private:
        std::string _msg;
};

#endif // IRCEXCEPTION_HPP
