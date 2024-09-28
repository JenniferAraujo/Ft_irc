#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Includes.hpp"

class Server;

//? :<Client> <Numeric Code> <Nick Intended>: <Msg Content>
inline std::string RPL_WELCOME(const std::string& source, const std::string& networkName, const std::string& nick, const std::string& user, const std::string& host) {
    return ":" + source + " 001 " + nick + " :Welcome to the " + networkName + " Network, " + nick + "!" + user + "@" + host + "\r\n";
}
inline std::string RPL_YOURHOST(const std::string& source, const std::string& serverName, const std::string& target, const std::string& version) {
    return ":" + source + " 002 " + target + " :Your host is " + serverName + ", running version " + version + "\r\n";
}
inline std::string RPL_CREATED(const std::string& source, const std::string& dateTime, const std::string& target) {
    return ":" + source + " 003 " + target + " :This server was created " + dateTime + "\r\n";
}
inline std::string RPL_MYINFO(const std::string& source, const std::string& target, const std::string& serverName, const std::string& version) {
    return ":" + source + " 004 " + target + " " + serverName + " " + version + " NCcCtu\r\n";
}

//:irc.example.com 005 YourNick PREFIX=(o)@ CHANTYPES=# CHANMODES=i,t,k,l,o STATUSMSG=@ USERLEN=12 :are supported by this server
inline std::string RPL_ISUPPORT(const std::string& source, const std::string& target) {
    std::stringstream ss;
    ss << ":" << source << " 005 " << target 
        << " CHANTYPES=" << CHANTYPES 
        << " CHANMODES=" << CHANMODES
        << " PREFIX=" <<  PREFIX 
        << " STATUSMSG=" << STATUSMSG
        << " USERLEN=" << USERLEN 
        << " :are supported by this server\r\n";
    return ss.str();
}

inline std::string RPL_MOTDSTART(const std::string& source, const std::string& target, const std::string& serverName) {
    return ":" + source + " 375 " + target + " :" + serverName + " Message of the day\r\n";
}
inline std::string RPL_MOTD(const std::string& source, const std::string& msg, const std::string& target) {
    return ":" + source + " 372 " + target + " :" + msg + "\r\n";
}
inline std::string RPL_ENDOFMOTD(const std::string& source, const std::string& target) {
    return ":" + source + " 376 " + target + " :End of /MOTD command.\r\n";
}

class Client {
public:
    Client(Server &server, time_t lastActivityTime); // Constructor
    Client();
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
    ACommand*                   createKick(std::istringstream &input);
    ACommand*                   createPart(std::istringstream &input);
    ACommand*                   createInvite(std::istringstream &input);
    ACommand*                   createTopic(std::istringstream &input);
    ACommand*                   createPrivmsg(std::istringstream &input);
    ACommand*                   createNotice(std::istringstream &input);
    ACommand*                   createQuit(std::istringstream &input);

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
    bool            getJustJoined(std::string channel) { return _justJoined[channel]; } ;

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
    void            setJustJoined(bool justJoined, std::string channel) { _justJoined[channel] = justJoined; } ;


private:
    int                     _socketFD;
    bool                    _regError;
    bool                    _registration;
    std::string             _ipAddr;
    std::string             _hostname;
    std::string             _password;
    std::string             _nick;
    std::string             _username;
    std::string             _realname;
    Server&                 _server;
    time_t                  _lastActivityTime;
    time_t                  _connectTime;
    std::map <std::string, bool>         _justJoined;
};


#endif