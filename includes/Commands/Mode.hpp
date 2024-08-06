#ifndef MODE_HPP
# define MODE_HPP

# include "Includes.hpp"

class Mode: public ACommand {
public:
    Mode(const Server& server, const Client& client);
    virtual ~Mode() {};

    void parsing(std::istringstream &input);

    std::string getChannel() { return _channel; };
private:
    Mode();
    std::string _channel;

};


#endif // MODE_HPP