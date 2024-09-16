#ifndef SERVER_HPP
# define SERVER_HPP

# include "Includes.hpp"

class Client;
class Channel;
class ACommand;

inline std::string ERROR(const std::string & msg) {
    return "ERROR :" + msg + "\r\n";
}

inline std::string ERR_NOTREGISTERED(const std::string& source, const std::string& target) {
    if(target.empty())
        return ":" + source + " 451 " + "*" + " :You have not registered\r\n";
    return ":" + source + " 451 " + target + " :You have not registered\r\n";
}

inline std::string QUIT(const std::string& nick, const std::string& user, const std::string& host, const std::string& msg) {
    return ":" + nick + "!" + user + "@" + host + " QUIT :" + msg + "\r\n";
}

class Server {
public:
	Server(const int &port, const std::string &password);
	~Server();
	Server(const Server& cpy);

    void run();
    void updateNFDs(int fd);
    void updateClients(Client *client, int fd);
    void updateToRemove(int fd, std::string error);
    void checkEvents(int nEvents);
    void verifyEvent(const pollfd &pfd);
    void executeCommand(Client &client, ACommand *command);
    void handleCommand(Client &client, std::vector<char> &buf);
    void handleTimeouts(time_t inactivityTimeout, time_t connectionTimeout);

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
    int    addInChannel(std::string channelName, std::string password, Client &client);
    Client  *findClient(std::string nick, int skipFd);
    int     getClientByNick(std::string nick);
    void    display() const;
    void    printChannelInfo(std::string channelName); //!SECTION
    void	signals();
    void    stopCompilation(int signal);

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
