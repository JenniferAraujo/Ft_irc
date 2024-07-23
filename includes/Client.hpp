#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Includes.hpp"

class Server;

class Client {
public:
    ~Client(); // Destructor
    Client(sockaddr_in6 socketInfo);

    static void     verifyConnection(Server &server, std::vector<pollfd>::iterator it);
    void            parseInfo(std::vector<char> buf);
    //getters
    int             getSocketFD() const;
    sockaddr_in6    getSocketInfo() const;
    std::string     getNick() const;
    std::string     getName() const;
    std::string     getRealName() const;

private:
    Client(); // Constructor

    int                     _socketFD;
    sockaddr_in6            _socketInfo;
    std::string             _nick;
    std::string             _name;
    std::string             _realName;

};


#endif // CLIENT_HPP
