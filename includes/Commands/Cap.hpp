#ifndef CAP_HPP
# define CAP_HPP

# include "Includes.hpp"

inline std::string CLIENT_NEGOTIATION(const std::string& client) {
    return ":" + client + " CAP * LS :\r\n";
}

class Cap: public ACommand {
public:
    Cap(Server& server, Client& client);
    ~Cap() {};
    void parsing(std::istringstream &input);
    void execute();
    void print() const;

private:
    Cap();
    //A minha ideia e ter uma variavel por argumento do commando
};

#endif // CAP_HPP