#ifndef SERVER_HPP
# define SERVER_HPP

# include "Includes.hpp"

class Server {
public:
    Server();
    Server(const int &port, const std::string &password);
    ~Server();

private:
    int         _port;
    std::string _password;

};

#endif // SERVER_HPP
