#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

# include "Includes.hpp"

//A class abstrata ACommand tem uma string com o nome do comando e um int que guarda o erro de comando, se necessario, inicializado a 0 -> sem erro
class ACommand {
public:
    ACommand(std::string command, Server server, Client client): _name(command), _server(server), _client(client), _error(0) {};
    virtual ~ACommand() {};
    std::string getName() const { return _name; };
    bool getError() const { return _error; };
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