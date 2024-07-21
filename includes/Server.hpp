#ifndef SERVER_HPP
# define SERVER_HPP

# include "Includes.hpp"

class Client;

class Server {
public:
    Server(const int &port, const std::string &password);
    ~Server();

    void run();
    void updateNFDs(int fd);
    void checkEvents();
    void verifyEvent();

private:
    Server();

    int                     _port;
    std::string             _password;
    int                     _socketFD;
    sockaddr_in6            _socketInfo;
    std::vector<pollfd>     _NFDs;
    std::map<int, Client*>   _Clients;

};

std::ostream &operator<<(std::ostream &out, const std::vector<pollfd>::iterator &it);


#endif // SERVER_HPP
