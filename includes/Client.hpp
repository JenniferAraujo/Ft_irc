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
    bool            parsePassword(std::istringstream &input, std::string str);
    bool            parseNick(std::istringstream &input, std::string str);
    bool            parseUser(std::istringstream &input, std::string str);

    //getters
    int             getSocketFD() const { return _socketFD; } ;
    bool            getValidCmd() const { return _validCmd; } ;
    bool            getPassward() const { return _passward; } ;
    std::string     getNick() const { return _nick; } ;
    std::string     getUsername() const { return _username; } ;
    std::string     getRealname() const { return _realname; } ;
    std::string     getCommand() const { return _command; } ;
    //setters
    void            setSocketFD(int socketFD) { _socketFD = socketFD; } ;
    void            setValidCmd(bool validCmd) { _validCmd = validCmd; } ;
    void            setPassword(bool passward) { _passward = passward; } ;
    void            setNick(std::string nick) { _nick = nick; } ;
    void            setUsername(std::string username) { _username = username; } ;
    void            setRealname(std::string realname) { _realname = realname; } ;
    void            setCommand(std::string command) { _command = command; } ;

private:
    Client();
    int                     _socketFD;
    bool                    _validCmd;
    bool                    _passward;
    std::string             _nick;
    std::string             _username;
    std::string             _realname;
    std::string             _command;
    Server&                 _server;
};

#endif // CLIENT_HPP