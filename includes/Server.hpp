#ifndef SERVER_HPP
# define SERVER_HPP

# include "Includes.hpp"

class Server {
public:
    Server(); // Constructor
    ~Server(); // Destructor

private:
    int         _port;
    std::string _password;

};

#endif // SERVER_HPP
