#ifndef PRIVMSG_HPP
# define PRIVMSG_HPP

# include "Includes.hpp"

//? :<Nick>!<User>@<Host> PRIVMSG <target> :<Msg Content>
inline std::string PRIV_MESSAGE(const std::string& nick, const std::string& user, const std::string& host, const std::string& target, const std::string& msg) {
    return ":" + nick + "!" + user + "@" + host + " PRIVMSG "  + target + " " + msg + "\r\n";
}

//so podes mandar msg a alguem no mesmo canal que tu?
//podes mandar msg para ti mesmo?
class Privmsg: public ACommand {
public:
    Privmsg(Server& server, Client& client);
    ~Privmsg() {};

    void parsing(std::istringstream &input);
    void parsingToken(std::string token);

    void sendToOpChannels();
    void sendToChannels();
    void sendToClients();
    void execute();

    void print() const;

    std::queue<std::string> getChannels() { return _channels; };
    std::queue<std::string> getOpChannels() { return _opChannels; };
    std::queue<std::string> getClients() { return _clients; };

private:
    Privmsg();
    std::queue<std::string> _channels;
    std::queue<std::string> _opChannels;
    std::queue<std::string> _clients;
    std::string             _message;
};

#endif