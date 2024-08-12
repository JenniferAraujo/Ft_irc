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
    bool    getEnd() const { return _end; };
    void parsing(std::istringstream &input);
    void execute();

private:
    Cap();
    bool _end;
    //A minha ideia e ter uma variavel por argumento do commando
};

#endif // CAP_HPP