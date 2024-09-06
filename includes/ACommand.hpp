#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

# include "Includes.hpp"

# define UNKNOWNERROR       400
# define NOSUCHNICK         401 //execuçao
# define NOSUCHCHANNEL      403 //execuçao
# define TOOMANYTARGETS     407
# define NORECIPIENT        411
# define NOTEXTTOSEND       412
# define UNKNOWNCOMMAND     421
# define NEEDMOREPARAMS     461

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

inline std::string ERR_NEEDMOREPARAMS(const std::string& source, const std::string& target, const std::string& command) {
    if(target.empty())
        return ":" + source + " 461 " + "*" + " " + command + " :Not enough parameters\r\n";
    return ":" + source + " 461 " + target + " " + command + " :Not enough parameters\r\n";
}

inline std::string ERR_ALREADYREGISTERED(const std::string& source, const std::string& target) {
    return ":" + source + " 462 " + target + " :You may not reregister\r\n";
}

//A class abstrata ACommand tem uma string com o nome do comando e um int que guarda o erro de comando, se necessario, inicializado a 0 -> sem erro
class ACommand {
public:
    ACommand(std::string command, Server &server, Client &client): _name(command), _server(server), _client(client), _error(0) {};
    //TODO copy
    virtual ~ACommand() {};
    std::string getName() const { return _name; };
    int getError() const { return _error; };
    bool isRegistration(){
        std::string commands[] = {"CAP", "PASS", "NICK", "USER"};
        for (int i = 0; i < 4; i++) {
            if(this->getName() == commands[i])
                return true;
        }
        return false;
    }
    bool existentChannel(std::string channelName) {
        //std::cout << "Checking channelName: '" << channelName << "'" << std::endl;
        std::map<std::string, Channel *> channels = this->_server.getChannels();
        //std::cout << "Total channels in map: " << channels.size() << std::endl;
        std::map<std::string, Channel *>::iterator it = channels.find(channelName);
        if (it != channels.end()) {
            //std::cout << "Found channel: " << it->first << std::endl;
            return true;
        }
        //std::cout << "Channel not found: " << channelName << std::endl;
        return false;
    }
    bool existentClient(std::string nickname) {
        std::map<int, Client*>::iterator it;
        std::map<int, Client*> clients = this->_server.getClients();

        for (it = clients.begin(); it != clients.end(); it++) {
            std::string clientNick = it->second->getNick();
            if (clientNick == nickname)
                return true;
        }
        return false;
    }
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