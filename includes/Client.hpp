#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Includes.hpp"

class Server;

class Client {
public:
    ~Client(); // Destructor
    Client(sockaddr_in6 socketInfo);

    static void verifyConnection(Server server, std::vector<pollfd>::iterator it);
    void getInfo();

private:
    Client(); // Constructor

    int                     _socketFD;
    sockaddr_in6            _socketInfo;

};

#endif // CLIENT_HPP
