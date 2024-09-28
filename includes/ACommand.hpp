#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

# include "Includes.hpp"

# define UNKNOWNERROR       400
# define NOSUCHNICK         401
# define NOSUCHCHANNEL      403
# define TOOMANYTARGETS     407
# define NORECIPIENT        411
# define NOTEXTTOSEND       412
# define UNKNOWNCOMMAND     421
# define USERNOTINCHANNEL   441
# define NOTONCHANNEL       442
# define USERONCHANNEL      443
# define NEEDMOREPARAMS     461
# define CHANNELISFULL      471
# define UNKNOWNMODE        472
# define BADCHANNELKEY      475
# define BADCHANMASK        476
# define CHANOPRIVSNEEDED   482
# define INVITEONLYCHAN     473
# define INVALIDMODEPARAM   696

inline std::string ERR_UNKNOWNERROR(const std::string& source, const std::string& target, const std::string& command, const std::string& info) {
    std::string _target = target;
    if(target.empty())
        _target = "*";
    if(command.empty())
        return ":" + source + " 400 " + _target + " :" + info + "\r\n";
    return ":" + source + " 400 " + _target + " " + command + " :" + info + "\r\n";
}

inline std::string ERR_NOSUCHNICK(const std::string& source, const std::string& target, const std::string& nick) {
    return ":" + source + " 401 " + target + " " + nick + " :No such nick\r\n";
}

inline std::string ERR_NOSUCHCHANNEL(const std::string& source, const std::string& target, const std::string& channel) {
    return ":" + source + " 403 " + target + " " + channel + " :No such channel\r\n";
}

inline std::string ERR_CANNOTSENDTOCHAN(const std::string& source, const std::string& target, const std::string& channel) {
    return ":" + source + " 404 " + target + " " + channel + " :Cannot send to channel\r\n";
}

inline std::string ERR_TOOMANYTARGETS(const std::string& source, const std::string& target, const std::string& command) {
    return ":" + source + " 407 " + target + " " + command + " :Too many targets\r\n";
}

inline std::string ERR_NORECIPIENT(const std::string& source, const std::string& target, const std::string& command) {
    return ":" + source + " 411 " + target + " :No recipient given (" + command + ")\r\n";
}

inline std::string ERR_NOTEXTTOSEND(const std::string& source, const std::string& target) {
    return ":" + source + " 412 " + target + " :No text to send\r\n";
}

inline std::string ERR_UNKNOWNCOMMAND(const std::string& source, const std::string& target, const std::string& command) {
    if(target.empty())
        return ":" + source + " 421 " + "*" + " " + command + " :Unknown command\r\n";
    return ":" + source + " 421 " + target + " " + command + " :Unknown command\r\n";
}

inline std::string ERR_USERNOTINCHANNEL(const std::string& source, const std::string& target, const std::string& nick, const std::string& channel) {
    return ":" + source + " 441 " + target + " " + nick + " " + channel + " :is not on that channel\r\n";
}

inline std::string ERR_NOTONCHANNEL(const std::string& source, const std::string& target, const std::string& channel) {
    return ":" + source + " 442 " + target  + " " + channel + " :You're not on that channel\r\n";
}

inline std::string ERR_USERONCHANNEL(const std::string& source,const std::string& client, const std::string& target, const std::string& channel) {
    return ":" + source + " 443 " + client + " " + target  + " " + channel + " :is already on channel\r\n";
}

inline std::string ERR_NEEDMOREPARAMS(const std::string& source, const std::string& target, const std::string& command) {
    if(target.empty())
        return ":" + source + " 461 " + "*" + " " + command + " :Not enough parameters\r\n";
    return ":" + source + " 461 " + target + " " + command + " :Not enough parameters\r\n";
}

inline std::string ERR_ALREADYREGISTERED(const std::string& source, const std::string& target) {
    return ":" + source + " 462 " + target + " :You may not reregister\r\n";
}

inline std::string ERR_CHANNELISFULL(const std::string& source, const std::string& target, const std::string& channel) {
    return ":" + source + " 471 " + target + " " + channel + " :Cannot join channel (+l)\r\n";
}

inline std::string ERR_UNKNOWNMODE(const std::string& source, const std::string& target, const std::string& modeChar) {
    return ":" + source + " 472 " + target + " " + modeChar + " :is unknown mode char to me\r\n";
}

inline std::string ERR_INVITEONLYCHAN(const std::string& source, const std::string& target, const std::string& channel) {
    return ":" + source + " 473 " + target + " " + channel + " :Cannot join channel (+i)\r\n";
}

inline std::string ERR_BADCHANNELKEY(const std::string& source, const std::string& target, const std::string& channel) {
    return ":" + source + " 475 " + target + " " + channel + " :Cannot join channel (+k)\r\n";
}

inline std::string ERR_CHANOPRIVSNEEDED(const std::string& source, const std::string& target, const std::string& channel) {
    return ":" + source + " 482 " + target + " " + channel + " :You're not channel operator\r\n";
}

inline std::string ERR_INVALIDMODEPARAM(const std::string& source, const std::string& target, const std::string& channel, const std::string& mode, const std::string& msg) {
    //tirar depois os sinais pq tem que mandar so o char
/*     if (mode == "k" || mode == "+k") {
        return ":" + source + " 696 " + target + " " + channel + " :You must specify a parameter for the key mode\r\n";
    }
    else if (mode == "l" || mode == "+l")
        return ":" + source + " 696 " + target + " " + channel + " :You must specify a parameter for the limit mode\r\n";
    else if (mode == "o" || mode == "+o" || mode == "-o") {
        return ":" + source + " 696 " + target + " " + channel + " :You must specify a parameter for the op mode\r\n";
    } */
    return ":" + source + " 696 " + target + " " + channel + " " + mode + " :" + msg + "\r\n";
}

//A class abstrata ACommand tem uma string com o nome do comando e um int que guarda o erro de comando, se necessario, inicializado a 0 -> sem erro
class ACommand {
public:
    ACommand(std::string command, Server &server, Client &client): _name(command), _server(server), _client(client), _error(0) {};
    //TODO copy
    virtual ~ACommand() {};
    std::string getName() const { return _name; };
    int getError() const { return _error; };
    bool isRegistration();
    bool existentChannel(std::string channelName);
    bool existentClient(std::string nickname);
    bool findInQueue(std::queue<std::string> q, const std::string& target);
    bool existentClientOnChannel(const std::string &nickname, const std::string &channelName);
    virtual void parsing(std::istringstream &input) = 0;
    virtual void execute() = 0;
    virtual void print() const = 0;

protected:
    std::string _name;
    Server      &_server;
    Client      &_client;
    int         _error;
};

#endif // PARSER_HPP