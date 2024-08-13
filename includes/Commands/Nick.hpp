#ifndef NICK_HPP
# define NICK_HPP

# include "Includes.hpp"

class Nick: public ACommand {
public:
    Nick(Server& server, Client& client);
    ~Nick() {};
    std::string getNick() { return _nick; };
    void parsing(std::istringstream &input);
    void execute();
    void print() const;

private:
    Nick();
    std::string _nick;
};

#endif // CAP_HPP