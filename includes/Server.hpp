#ifndef SERVER_HPP
# define SERVER_HPP

# include "Includes.hpp"

class Client;
class Channel;

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
    void join(const Client &client);

    //getters
    int getSocketFD() const { return this->_socketFD; };
    int getPort() const { return this->_port; };
    sockaddr_in6 getSocketInfo() const { return this->_socketInfo; };
    std::string getPassword() const { return _password; };
    std::string getCreationTime() const {return this->_creationTime;};

    //aux
    void    removeClient(int fd);
    void    getServerInfo();

private:
    Server();

    int                     _port;
    std::string             _password;
    char *                   _hostName;
    char *                  _hostIP;
    int                     _socketFD;
    std::string             _creationTime;
    sockaddr_in6            _socketInfo;
    std::vector<pollfd>     _NFDs;
    std::map<int, Client*>   _Clients;
    std::map<std::string, Channel*>   _Channels;

};

#endif // SERVER_HPP
