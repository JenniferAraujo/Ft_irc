#ifndef KICK_HPP
# define KICK_HPP

# include "Includes.hpp"

# define KICKDEFAULTMSG "Behave yourself please"

inline std::string KICK(const std::string& nick, const std::string& user, const std::string& host, const std::string& channel, const std::string& nickKicked, const std::string& reason) {
    return ":" + nick + "!" + user + "@" + host + " KICK " + channel +  " " + nickKicked + " :" + reason + "\r\n";
}

class Kick: public ACommand {
public:
    Kick(Server& server, Client& client);
    ~Kick() {};

    void parsing(std::istringstream &input);
    void execute();
    void print() const;

    std::string getChannel() { return _channel; };
    std::string getClient() { return _cliente; };
    std::string getReason() { return _reason; };

private:
    Kick();
    std::string _channel;
    std::string _cliente;
    std::string _reason;
};

#endif // KICK_HPP