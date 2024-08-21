#ifndef PASS_HPP
# define PASS_HPP

# include "Includes.hpp"
# define PASSWDMISMATCH 464

inline std::string ERR_PASSWDMISMATCH(const std::string& client, const std::string& nick) {
    return ":" + client + " 462 " + nick + " :Password incorrect\r\n";
}

class Pass: public ACommand {
public:
    Pass(Server& server, Client& client);
    ~Pass() {};

    std::string getPass() { return _pass; };
    void parsing(std::istringstream &input);
    void execute();
    void print() const;

private:
    std::string _pass;
    //A minha ideia e ter uma variavel por argumento do commando
};

#endif // CAP_HPP