#ifndef NICK_HPP
# define NICK_HPP

# include "Includes.hpp"

# define NONICKNAMEGIVEN 431
# define ERRONEUSNICKNAME 432
# define NICKNAMEINUSE 433

inline std::string ERR_NONICKNAMEGIVEN(const std::string& client, const std::string& nick) {
    return ":" + client + " 431 " + nick + " :No nickname given\r\n";
}

inline std::string ERR_ERRONEUSNICKNAME(const std::string& client, const std::string& nick, const std::string& new_nick) {
    return ":" + client + " 432 " + nick + " " + new_nick + " :Erroneus nickname\r\n";
}

inline std::string ERR_NICKNAMEINUSE(const std::string& client, const std::string& nick, const std::string& new_nick) {
    return ":" + client + " 433 " + nick + " " + new_nick + " :Nickname is already in use\r\n";
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