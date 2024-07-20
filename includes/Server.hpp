#ifndef SERVER_HPP
# define SERVER_HPP

# include "Includes.hpp"

class Server {
public:
    Server();
    Server(const int &port, const std::string &password);
    ~Server();

    void run();

private:
    int             _port;
    std::string     _password;
    int             _socketFD;
    sockaddr_in6    _socketInfo;

};

#endif // SERVER_HPP
