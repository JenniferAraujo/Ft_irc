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

	//aux commands
	bool	hasPassword(const std::string& password) const;
	bool	isInvited(const std::string& name) const;
	void	addInvite(const std::string& name);
	void	setEnteredPassword(const std::string& password);
	
    //getters
    int             getSocketFD() const { return _socketFD; } ;
    int             getRegError() const { return _regError; } ;
    bool            getRegistration() const { return _registration; } ;
    bool            getCap() const { return _cap; } ;
    bool            getCapend() const { return _capEnd; } ;
    std::string     getPassword() const { return _password; } ;
    std::string     getNick() const { return _nick; } ;
    std::string     getUsername() const { return _username; } ;
    std::string     getRealname() const { return _realname; } ;
    std::string     getIpaddr() const { return _ipAddr; } ;
    Server          &getServer() const { return _server; } ;

    //setters
    void            setSocketFD(int socketFD) { _socketFD = socketFD; } ;
    void            setRegError(int regError) { _regError = regError; } ;
    void            setPass(std::string password) { _password = password; } ;
    void            setNick(std::string nick) { _nick = nick; } ;
    void            setUsername(std::string username) { _username = username; } ;
    void            setRealname(std::string realname) { _realname = realname; } ;
    void            setIpAddr(std::string ipAddr) { _ipAddr = ipAddr; } ;
    void            setRegistration(bool registration) { _registration = registration; } ;
    void            setCapend(bool capEnd) { _capEnd = capEnd; } ;
    void            setCap(bool cap) { _cap = cap; } ;

private:
    Client();
    int                     _socketFD;
    int                     _regError; //Possivelmente mudar para bool
    bool                    _registration; //initialized as false
	std::vector<std::string> _invitedNames; //NOTE - clientes que foram convidados p entrar em canal
	std::string				_enteredPassword; //NOTE -  password p entrar no canal
    bool                    _cap;
    bool                    _capEnd;
    std::string             _ipAddr;
    std::string             _password;
    std::string             _nick;
    std::string             _username;
    std::string             _realname;
    Server&                 _server;
};

#endif // CLIENT_HPP