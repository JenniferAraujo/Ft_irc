#ifndef NICK_HPP
# define NICK_HPP

# include "Includes.hpp"

class Nick: public ACommand {
public:
    Nick(): ACommand("NICK") {};
    ~Nick() {};
    std::string getNick() { return _nick; };
    //assim?
    void parsing();

private:
    Nick();
    std::string _nick;
};

#endif // CAP_HPP