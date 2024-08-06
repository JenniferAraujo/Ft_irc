#ifndef NICK_HPP
# define NICK_HPP

# include "Includes.hpp"

class Nick: public ACommand {
public:
    Nick(const Server& server, const Client& client);
    ~Nick();
    std::string getNick() { return _nick; };
    void parsing(std::istringstream &input);

private:
    Nick();
    std::string _nick;
};

#endif // CAP_HPP