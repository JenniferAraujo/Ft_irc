#ifndef WHO_HPP
# define WHO_HPP

# include "Includes.hpp"

class Who: public ACommand {
public:
    Who(Server& server, Client& client);
    ~Who() {};

    void parsing(std::istringstream &input);
    void execute();
    void print() const;

    std::string getChannel() { return _channel; };
private:
    Who();
    std::string _channel;

};


#endif // WHO_HPP