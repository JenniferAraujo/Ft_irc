#ifndef PASS_HPP
# define PASS_HPP

# include "Includes.hpp"

class Pass: public ACommand {
public:
    Pass(const Server& server, const Client& client);
    ~Pass();

    std::string getPass() { return _pass; };
    void parsing(std::istringstream &input);

private:
    std::string _pass;
    //A minha ideia e ter uma variavel por argumento do commando
};

#endif // CAP_HPP