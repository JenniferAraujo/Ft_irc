#ifndef CAP_HPP
# define CAP_HPP

# include "Includes.hpp"

class Cap: public ACommand {
public:
    Cap(): ACommand("CAP") {};
    virtual ~Cap() {};

    //assim?
    void parsing();

private:
    Cap();
    bool _end;
    //A minha ideia e ter uma variavel por argumento do commando
};

#endif // CAP_HPP