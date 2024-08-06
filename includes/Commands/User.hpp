#ifndef USER_HPP
# define USER_HPP

# include "Includes.hpp"

class User: public ACommand {
public:
    User(): ACommand("USER") {};
    virtual ~User() {};

    //assim?
    std::string getName() { return _name; };
    std::string getRealname() { return _realname; };
    void parsing();

private:
    User();
    std::string _name;
    std::string _realname;
    //A minha ideia e ter uma variavel por argumento do commando
};

#endif // CAP_HPP