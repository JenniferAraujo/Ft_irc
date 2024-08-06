#ifndef CAP_HPP
# define CAP_HPP

# include "Includes.hpp"

class Cap: public ACommand {
public:
    Cap(const Server& server, const Client& client);
    virtual ~Cap() {};

    void parsing(std::istringstream &input);

private:
    Cap();
    bool _end;
    //A minha ideia e ter uma variavel por argumento do commando
};

#endif // CAP_HPP