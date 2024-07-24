#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Includes.hpp"

class Server;

class Client {
public:
    Client(Server &server); // Constructor
    ~Client(); // Destructor

    static void     verifyConnection(Server &server, const pollfd &pfd);
    void            parseMessage(std::vector<char> buf);
    bool            parseClient(std::istringstream &input, std::string str);

    //getters
    int             getSocketFD() const { return _socketFD; } ;
    std::string     getNick() const { return _nick; } ;
    std::string     getUser() const { return _user; } ;
    std::string     getName() const { return _name; } ;
    std::string     getRealName() const { return _realName; } ;
    bool            getValidCmd() const { return _validCmd; } ;
    std::string     getCommand() const { return _command; } ;

private:
    Client();
    int                     _socketFD;
    bool                    _validCmd;
    std::string             _nick;
    std::string             _name;
    std::string             _user;
    std::string             _realName;
    std::string             _command;
    Server&                 _server;
};

#endif // CLIENT_HPP
