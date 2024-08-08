#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

# include "Includes.hpp"

//A class abstrata ACommand tem uma string com o nome do comando e um int que guarda o erro de comando, se necessario, inicializado a 0 -> sem erro
class ACommand {
public:
    ACommand(std::string command, Server server, Client client): _name(command), _server(server), _client(client), _error(0) {};
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
    virtual void parsing(std::istringstream &input) = 0;

protected:
    std::string _name;
    //Nao sao referencias nem ponteiros, servem so para ir buscar info, nao para modificar
    Server      _server;
    Client      _client;
    //TODO criar macros
    int         _error;
};

#endif // PARSER_HPP