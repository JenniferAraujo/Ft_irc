#ifndef MODE_HPP
# define MODE_HPP

# include "Includes.hpp"

class Mode: public ACommand {
public:
    Mode(const Server& server, const Client& client);
    ~Mode() {};

    void parsing(std::istringstream &input);

    std::string getChannel() { return _channel; };
	std::string getMode() { return _mode; };
private:
    Mode();
    std::string _channel;
	std::string _mode;
};


#endif // MODE_HPP