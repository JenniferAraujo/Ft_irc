#ifndef SERVER_HPP
# define SERVER_HPP

# include "Includes.hpp"

class Client;

class Server {
public:
    Server(const int &port, const std::string &password);
    ~Server();

    void run();
    void getAddrInfo();
    void updateNFDs(int fd);
    void updateClients(Client *client, int fd);
    void checkEvents(int nEvents);
    void verifyEvent(const pollfd &pfd);
    void executeCommand(Client &client);
    void cap(const Client &client);

    int getSocketFD() const;
    int getPort() const;
    sockaddr_in6 getSocketInfo() const;
    std::string getCreationTime() const {return this->_creationTime;};

private:
    Server();

    int                     _port;
    std::string             _password;
    int                     _socketFD;
    std::string             _creationTime;
    sockaddr_in6            _socketInfo;
    std::vector<pollfd>     _NFDs;
    std::map<int, Client*>   _Clients;

};

#endif // SERVER_HPP
