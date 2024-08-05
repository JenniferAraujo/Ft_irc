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
    void welcome(Client &client);
    void join(Client &client);
    void mode(Client &client);
    void who(Client &client);

    //getters
    int getSocketFD() const { return this->_socketFD; };
    int getPort() const { return this->_port; };
    std::string getHostname() const { return this->_hostName; };
    sockaddr_in6 getSocketInfo() const { return this->_socketInfo; };
    std::string getPassword() const { return _password; };
    std::string getCreationTime() const {return this->_creationTime;};
    //aux
    void    removeClient(int fd);
    void    getServerInfo();
    void    addInChannel(std::string channelName, Client &client);

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
    std::vector <int>       _toRemove;

};

#endif // SERVER_HPP
