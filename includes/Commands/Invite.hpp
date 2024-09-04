#ifndef INVITE_HPP
# define INVITE_HPP

# include "Includes.hpp"

inline std::string INVITE(const std::string& nick, const std::string& channel, const std::string& nickInvited) {
    return ":" + nick + " INVITE " + nickInvited + " :" + channel + "\r\n";
}

inline std::string RPL_INVITING(const std::string& source, const std::string& nick, const std::string& nickInvited, const std::string& channel) {
    return ":" + source + " 341 " + nick + " " + nickInvited + " " + channel + "\r\n";
}

class Invite: public ACommand {
public:
    Invite(Server& server, Client& client);
    ~Invite() {};

    void parsing(std::istringstream &input);
    void execute();
    void print() const;

    std::string getChannel() { return _channel; };
    std::string getNickname() { return _nickname; };

private:
    Invite();
    std::string _channel;
    std::string _nickname;
};

#endif // INVITE_HPP