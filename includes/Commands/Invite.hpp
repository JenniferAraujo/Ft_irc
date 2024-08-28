#ifndef INVITE_HPP
# define INVITE_HPP

# include "Includes.hpp"

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