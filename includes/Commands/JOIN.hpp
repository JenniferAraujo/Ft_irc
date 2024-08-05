#ifndef JOIN_HPP
# define JOIN_HPP

# include "Includes.hpp"

class Parser;

class JOIN: public Parser {
public:
    JOIN();
    virtual ~JOIN();

    //assim?
    void treatCommand();

private:
    //confirmar com a rita depois como ela vai querer fazer isto
    //vais guardar aqui o input seguinte para se saber o channel, decide como queres tratar
    //podes tambem ter flags de erro como todos os outros para verificar se aconteceu algum problema no JOIN
    std::map<std::string, std::string> _fullCmd;
};

#endif // JOIN_HPP