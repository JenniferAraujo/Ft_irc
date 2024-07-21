#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Includes.hpp"

class Server;

class Client {
public:
    ~Client(); // Destructor

    static void verifyConnection(Server server, std::vector<pollfd>::iterator it);

private:
    Client(); // Constructor

    int                     _socketFD;
    sockaddr_in6            _socketInfo;

};

#endif // CLIENT_HPP
