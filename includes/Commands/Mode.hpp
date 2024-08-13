#ifndef MODE_HPP
# define MODE_HPP

# include "Includes.hpp"

class Mode: public ACommand {
public:
    Mode(Server& server, Client& client);
    ~Mode() {};

    void parsing(std::istringstream &input);
    void execute();
    void print() const;

    std::string getChannel() { return _channel; };
private:
    Mode();
    std::string _channel;

};


#endif // MODE_HPP