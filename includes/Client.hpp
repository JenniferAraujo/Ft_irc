#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Includes.hpp"

class Server;

class Client {
public:
    Client(); // Constructor
    ~Client(); // Destructor

    static void     verifyConnection(Server &server, const pollfd &pfd);
    void            parseMessage(std::vector<char> buf);

    //getters
    int             getSocketFD() const;
    std::string     getNick() const;
    std::string     getName() const;
    std::string     getRealName() const;
    bool            getValidCmd() const { return _validCmd; } ;
    std::string     getCommand() const { return _command; } ;
    //setters
    void    setCommand(std::string command){this->_command = command;}

private:

    int                     _socketFD;
    bool                    _validCmd;
    std::string             _nick;
    std::string             _name;
    std::string             _realName;
    std::string             _command;
};


#endif // CLIENT_HPP
