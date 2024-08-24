#ifndef SERVER_HPP
# define SERVER_HPP

# include "Includes.hpp"

class Client;
class Channel;
class ACommand;

class Server {
public:
    Server(const int &port, const std::string &password);
    ~Server();
    Server(const Server& cpy);

    void run();
    void getAddrInfo();
    void updateNFDs(int fd);
    void updateClients(Client *client, int fd);
    void updateToRemove(int fd, std::string error);
    void checkEvents(int nEvents);
    void verifyEvent(const pollfd &pfd);
    void welcome(Client &client);
    void executeCommand(Client &client, ACommand *command);
    void handleCommand(Client &client, std::vector<char> &buf);

    //getters
    int getSocketFD() const { return this->_socketFD; };
    int getPort() const { return this->_port; };
    std::string getHostname() const { return this->_hostName; };
    std::string getHostIP() const { return this->_hostIP; };
    std::string getPassword() const { return _password; };
    std::string getCreationTime() const {return this->_creationTime; };
    sockaddr_in6 getSocketInfo() const { return this->_socketInfo; };
    std::vector<pollfd> getNFD() const { return this->_NFDs; };
    std::map<int, Client*> getClients() const { return this->_Clients; };
    std::map<std::string, Channel*> getChannels() const {return this->_Channels; };
    std::map <int, std::string>  getToRemove() const { return this->_toRemove; };

    //aux
    void    removeClient(int fd, std::string error);
    void    getServerInfo();
    void    addInChannel(std::string channelName, Client &client);
    Client  *findClient(std::string nick);

private:
    Server();

    int                                 _port;
    std::string                         _password;
    std::string                         _hostName;
    std::string                         _hostIP;
    int                                 _socketFD;
    std::string                         _creationTime;
    sockaddr_in6                        _socketInfo;
    std::vector<pollfd>                 _NFDs;
    std::map<int, Client*>              _Clients;
    std::map<std::string, Channel*>     _Channels;
    std::map <int, std::string>         _toRemove;
};

#endif // SERVER_HPP
