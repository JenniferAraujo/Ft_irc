#ifndef JOIN_HPP
# define JOIN_HPP

# include "Includes.hpp"

class Join: public ACommand {
public:
    Join(): ACommand("JOIN") {};
    virtual ~Join() {};

    //assim?
    void parsing();

private:
    Join();
    std::map<std::string, std::string> _fullCmd;
    //A minha ideia e ter uma variavel por argumento do commando
};

#endif // JOIN_HPP