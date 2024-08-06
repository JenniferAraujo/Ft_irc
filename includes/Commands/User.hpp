#ifndef USER_HPP
# define USER_HPP

# include "Includes.hpp"

class User: public ACommand {
public:
    User(const Server& server, const Client& client);
    virtual ~User() {};

    std::string getName() { return _name; };
    std::string getRealname() { return _realname; };
    void parsing(std::istringstream &input);

private:
    User();
    std::string _name;
    std::string _realname;
    //A minha ideia e ter uma variavel por argumento do commando
};

#endif // CAP_HPP