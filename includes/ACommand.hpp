#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

# include "Includes.hpp"

# define NEEDMOREPARAMS 461
# define UNKNOWNCOMMAND 421
# define UNKNOWNERROR   400

inline std::string ERR_NEEDMOREPARAMS(const std::string& client, const std::string& nick, const std::string& command) {
    return ":" + client + " 461 " + nick + " " + command + " :Not enough parameters\r\n";
}

inline std::string ERR_UNKNOWNCOMMAND(const std::string& client, const std::string& nick, const std::string& command) {
    return ":" + client + " 421 " + nick + " " + command + " :Unknown command\r\n";
}

inline std::string ERR_UNKNOWNERROR(const std::string& client, const std::string& nick, const std::string& command, const std::string& info) {
    return ":" + client + " 400 " + nick + " " + command + " :" + info + "\r\n";
}

inline std::string ERR_ALREADYREGISTERED(const std::string& client, const std::string& nick) {
    return ":" + client + " 462 " + nick + " :You may not reregister\r\n";
}


//A class abstrata ACommand tem uma string com o nome do comando e um int que guarda o erro de comando, se necessario, inicializado a 0 -> sem erro
class ACommand {
public:
    ACommand(std::string command, Server &server, Client &client): _name(command), _server(server), _client(client), _error(0) {};
    //TODO copy
    virtual ~ACommand() {};
    std::string getName() const { return _name; };
    int getError() const { return _error; };
    void trimChar(std::string& str, char ch) {
        std::string::size_type first = str.find_first_not_of(ch);
        std::string::size_type last = str.find_last_not_of(ch);
        if (first == std::string::npos)
            str.clear();
        else
            str = str.substr(first, last - first + 1);
    }

    bool isRegistration(){
        std::string commands[] = {"CAP", "PASS", "NICK", "USER"};
        for (int i = 0; i < 4; i++) {
            if(this->getName() == commands[i])
                return true;
        }
        return false;
    }
    virtual void parsing(std::istringstream &input) = 0;
    virtual void execute() = 0;
    virtual void print() const = 0;

protected:
    std::string _name;
    //Nao sao referencias nem ponteiros, servem so para ir buscar info, nao para modificar
    Server      &_server;
    Client      &_client;
    //TODO criar macros
    int         _error;
};

#endif // PARSER_HPP