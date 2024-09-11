#ifndef CAP_HPP
# define CAP_HPP

# include "Includes.hpp"

class Cap: public ACommand {
public:
    Cap(Server& server, Client& client);
    ~Cap() {};
    void parsing(std::istringstream &input);
    void execute();
    void print() const;

private:
    Cap();
};

#endif // CAP_HPP