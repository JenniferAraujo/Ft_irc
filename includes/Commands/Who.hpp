#ifndef WHO_HPP
# define WHO_HPP

# include "Includes.hpp"

class Parser;

class WHO: public Parser {
public:
    WHO();
    virtual ~WHO();

    //assim?
    void treatCommand();

private:
    //confirmar com a rita depois como ela vai querer fazer isto
    //vais guardar aqui o input seguinte para se saber o channel, decide como queres tratar
    //podes tambem ter flags de erro como todos os outros para verificar se aconteceu algum problema no JOIN
    std::map<std::string, std::string> _fullCmd;
};

#endif // WHO_HPP