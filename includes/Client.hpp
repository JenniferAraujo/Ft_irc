#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Includes.hpp"

class Server;

class Client {
public:
    Client(Server &server, time_t lastActivityTime); // Constructor
    ~Client(); // Destructor
    Client(const Client& cpy);

    static void     verifyConnection(Server &server, const pollfd &pfd);
    void            getClientInfo();

    //Registration
    void                        registration();
    void                        welcome();

    //Create Commands
    std::queue<ACommand* >      createCommand(std::vector<char> buf);
    ACommand*                   createCap(std::istringstream &input);
    ACommand*                   createPass(std::istringstream &input);
    ACommand*                   createNick(std::istringstream &input);
    ACommand*                   createUser(std::istringstream &input);
    ACommand*                   createJoin(std::istringstream &input);
    ACommand*                   createMode(std::istringstream &input);
    ACommand*                   createWho(std::istringstream &input);
    ACommand*                   createPing(std::istringstream &input);

    //getters
    int             getSocketFD() const { return _socketFD; } ;
    int             getRegError() const { return _regError; } ;
    bool            getRegistration() const { return _registration; } ;
    std::string     getPassword() const { return _password; } ;
    std::string     getNick() const { return _nick; } ;
    std::string     getUsername() const { return _username; } ;
    std::string     getRealname() const { return _realname; } ;
    std::string     getIpaddr() const { return _ipAddr; } ;
    Server          &getServer() const { return _server; } ;
    time_t          getLastActivityTime() const { return _lastActivityTime; } ;
    time_t          getConnectTime() const { return _connectTime; } ;

    //setters
    void            setSocketFD(int socketFD) { _socketFD = socketFD; } ;
    void            setRegError(int regError) { _regError = regError; } ;
    void            setPass(std::string password) { _password = password; } ;
    void            setNick(std::string nick) { _nick = nick; } ;
    void            setUsername(std::string username) { _username = username; } ;
    void            setRealname(std::string realname) { _realname = realname; } ;
    void            setIpAddr(std::string ipAddr) { _ipAddr = ipAddr; } ;
    void            setRegistration(bool registration) { _registration = registration; } ;
    void            setLastActivityTime(time_t lastActivityTime) { _lastActivityTime = lastActivityTime; } ;
    void            setConnectTime(time_t connectTime) { _connectTime = connectTime; } ;

private:
    Client();
    int                     _socketFD;
    bool                    _regError;
    bool                    _registration;
    std::string             _ipAddr;
    std::string             _password;
    std::string             _nick;
    std::string             _username;
    std::string             _realname;
    Server&                 _server;
    time_t                  _lastActivityTime;
    time_t                  _connectTime;
};


#endif // CLIENT_HPP