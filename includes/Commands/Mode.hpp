#ifndef MODE_HPP
# define MODE_HPP

# include "Includes.hpp"

class Mode: public ACommand {
public:
    Mode(): ACommand("MODE") {};
    virtual ~Mode() {};

    //assim?
    void parsing();

private:
    Mode();
    //TODO mudar para variaveis individuais
    std::map<std::string, std::string> _fullCmd;
    //A minha ideia e ter uma variavel por argumento do commando
};

#endif // MODE_HPP