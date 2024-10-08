#ifndef NICK_HPP
# define NICK_HPP

# include "Includes.hpp"

# define NONICKNAMEGIVEN 431
# define ERRONEUSNICKNAME 432
# define NICKNAMEINUSE 433

inline std::string ERR_NONICKNAMEGIVEN(const std::string& source, const std::string& target) {
    if(target.empty())
        return ":" + source + " 431 " + "*" + " :No nickname given\r\n";
    return ":" + source + " 431 " + target + " :No nickname given\r\n";
}

inline std::string ERR_ERRONEUSNICKNAME(const std::string& source, const std::string& target, const std::string& nick) {
    if(target.empty())
        return ":" + source + " 432 " + "*" + " " + nick + " :Erroneus nickname\r\n";
    return ":" + source + " 432 " + target + " " + nick + " :Erroneus nickname\r\n";
}

inline std::string ERR_NICKNAMEINUSE(const std::string& source, const std::string& target, const std::string& nick) {
    if(target.empty())
        return ":" + source + " 433 " + "*" + " " + nick + " :Nickname is already in use\r\n";
    return ":" + source + " 433 " + target + " " + nick + " :Nickname is already in use\r\n";
}

//? :<OldNick>!<User>@<Host> NICK <NewNick> 
//:dan-!d@localhost NICK Mamoped
inline std::string NICK_MESSAGE(const std::string& old_nick, const std::string& user, const std::string& host, const std::string& new_nick) {
    if(old_nick.empty() && user.empty())
        return ":*!*@" + host + " NICK "  + new_nick + "\r\n";
    if(old_nick.empty())
        return ":*!" + user + "@" + host + " NICK "  + new_nick + "\r\n";
    if(user.empty())
        return ":" + old_nick + "!*@" + host + " NICK "  + new_nick + "\r\n";
    return ":" + old_nick + "!" + user + "@" + host + " NICK "  + new_nick + "\r\n";
}

class Nick: public ACommand {
public:
    Nick(Server& server, Client& client);
    ~Nick() {};
    std::string getNick() { return _nick; };
    void parsing(std::istringstream &input);
    void execute();
    void print() const;

private:
    Nick();
    std::string _nick;
};

#endif // CAP_HPP