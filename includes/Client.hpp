#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Includes.hpp"

class Server;

class Client {
public:
    ~Client(); // Destructor

    static void verifyConnection(Server server);

private:
    Client(); // Constructor
};

#endif // CLIENT_HPP
