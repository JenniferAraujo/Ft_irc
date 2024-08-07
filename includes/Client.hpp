#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Includes.hpp"

class Server;

class Client {
public:
    Client(Server &server); // Constructor
    ~Client(); // Destructor
    Client(const Client& cpy);

    static void     verifyConnection(Server &server, const pollfd &pfd);
    void            getClientInfo();

    //Create Commands
    std::queue<ACommand* >      createCommand(std::vector<char> buf);
    ACommand*                   createCap(std::istringstream &input);
    ACommand*                   createPass(std::istringstream &input);
    ACommand*                   createNick(std::istringstream &input);
    ACommand*                   createUser(std::istringstream &input);
    ACommand*                   createJoin(std::istringstream &input);
    ACommand*                   createMode(std::istringstream &input);
    ACommand*                   createWho(std::istringstream &input);

    //Parsing da autentificaçao
    void            parsePassword(std::istringstream &input);
    void            parseNick(std::istringstream &input);
    void            parseUser(std::istringstream &input);

    //Parsing de commandos
    bool            parseCap(std::istringstream &input, std::string str);
    bool            parseMode(std::istringstream &input, std::string str);
    bool            parseWho(std::istringstream &input, std::string str);


    //getters
    int             getSocketFD() const { return _socketFD; } ;
    int             getAuthError() const { return _authError; } ;
    bool            getRegistration() const { return _registration; } ;
    std::string     getPassword() const { return _password; } ;
    std::string     getNick() const { return _nick; } ;
    std::string     getUsername() const { return _username; } ;
    std::string     getRealname() const { return _realname; } ;
    std::string     getIpaddr() const { return _ipAddr; } ;
    Server     &getServer() const { return _server; } ;
    //setters
    void            setSocketFD(int socketFD) { _socketFD = socketFD; } ;
    void            setAuthError(int authError) { _authError = authError; } ;
    void            setPass(std::string password) { _password = password; } ;
    void            setNick(std::string nick) { _nick = nick; } ;
    void            setUsername(std::string username) { _username = username; } ;
    void            setRealname(std::string realname) { _realname = realname; } ;
    void            setIpAddr(std::string ipAddr) { _ipAddr = ipAddr; } ;
    void            setRegistration(bool registration) { _registration = registration; } ;

private:
    Client();
    int                     _socketFD;
    int                     _authError; //Possivelmente mudar para bool
    bool                    _registration; //initialized as false
    std::string             _ipAddr;
    std::string             _password;
    std::string             _nick;
    std::string             _username;
    std::string             _realname;
    Server&                 _server;
};

#endif // CLIENT_HPP