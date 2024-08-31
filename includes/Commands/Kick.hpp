#ifndef KICK_HPP
# define KICK_HPP

# include "Includes.hpp"

# define BADCHANMASK 476
# define NOSUCHCHANNEL 403
# define USERNOTINCHANNEL 441

class Kick: public ACommand {
public:
    Kick(Server& server, Client& client);
    ~Kick() {};

    void parsing(std::istringstream &input);
    void execute();
    void print() const;

    std::string getChannel() { return _channel; };
    std::string getClient() { return _client; };
    std::string getReason() { return _reason; };

private:
    Kick();
    std::string _channel;
    std::string _client;
    std::string _reason;

};

#endif // KICK_HPP