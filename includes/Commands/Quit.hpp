#ifndef QUIT_HPP
# define QUIT_HPP

# include "Includes.hpp"

class Quit: public ACommand {
public:
    Quit(Server& server, Client& client);
    ~Quit() {};
    void parsing(std::istringstream &input);
    void execute();
    void print() const;

private:
    Quit();
    std::string _reason;
};

#endif // CAP_HPP