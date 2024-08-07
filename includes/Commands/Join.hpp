#ifndef JOIN_HPP
# define JOIN_HPP

# include "Includes.hpp"

class Join: public ACommand {
public:
    Join(const Server& server, const Client& client);
    ~Join() {};

    void parsing(std::istringstream &input);

    std::string getChannel() { return _channel; };

private:
    Join();
    std::string _channel;
};

#endif // JOIN_HPP