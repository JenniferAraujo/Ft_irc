#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Includes.hpp"

class Server;

class Client {
public:
    Client(Server &server); // Constructor
    ~Client(); // Destructor

    static void     verifyConnection(Server &server, const pollfd &pfd);

    //Parsing da mensagem
    void            parseMessage(std::vector<char> buf);

    //Parsing da autentificaçao
    void            parsePassword(std::istringstream &input);
    void            parseNick(std::istringstream &input);
    void            parseUser(std::istringstream &input);

    //Parsing de commandos
    bool            parseCap(std::istringstream &input, std::string str);
    bool            parseJoin(std::istringstream &input, std::string str);

    //getters
    int             getSocketFD() const { return _socketFD; } ;
    int             getAuthError() const { return _authError; } ;
    bool             getPassword() const { return _password; } ;
    bool            getAuthOver() const { return _authOver; } ;
    bool            getValidCmd() const { return _validCmd; } ;
    std::string     getNick() const { return _nick; } ;
    std::string     getUsername() const { return _username; } ;
    std::string     getRealname() const { return _realname; } ;
    std::string     getCommand() const { return _command; } ;
    //setters
    void            setSocketFD(int socketFD) { _socketFD = socketFD; } ;
    void            setValidCmd(bool validCmd) { _validCmd = validCmd; } ;
    void            setNick(std::string nick) { _nick = nick; } ;
    void            setUsername(std::string username) { _username = username; } ;
    void            setRealname(std::string realname) { _realname = realname; } ;
    void            setCommand(std::string command) { _command = command; } ;

private:
    Client();
    int                     _socketFD;
    int                     _authError;
    bool                    _password; //initialized as false
    bool                    _authOver; //initialized as false
    bool                    _validCmd; //initialized as false
    std::string             _nick;
    std::string             _username;
    std::string             _realname;
    std::string             _command;
    Server&                 _server;
};

#endif // CLIENT_HPP