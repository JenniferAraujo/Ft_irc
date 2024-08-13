#ifndef USER_HPP
# define USER_HPP

# include "Includes.hpp"

class User: public ACommand {
public:
    User(Server& server, Client& client);
    ~User() {};

    std::string getUsername() { return _username; };
    std::string getRealname() { return _realname; };
    void parsing(std::istringstream &input);
    void execute();
    void print() const;

private:
    User();
    std::string _username;
    std::string _realname;
    //A minha ideia e ter uma variavel por argumento do commando
};

#endif // CAP_HPP