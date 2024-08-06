#ifndef WHO_HPP
# define WHO_HPP

# include "Includes.hpp"

class Who: public ACommand {
public:
    Who(): ACommand("WHO") {};
    virtual ~Who() {};

    //assim?
    void parsing();

private:
    Who();
    //TODO mudar para variaveis individuais
    std::map<std::string, std::string> _fullCmd;
    //A minha ideia e ter uma variavel por argumento do commando
};


#endif // WHO_HPP