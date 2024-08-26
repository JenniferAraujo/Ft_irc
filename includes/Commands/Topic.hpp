#ifndef TOPIC_HPP
# define TOPIC_HPP

# include "Includes.hpp"

class Topic: public ACommand {
public:
    Topic(Server& server, Client& client);
    ~Topic() {};

    void parsing(std::istringstream &input);
    void execute();
    void print() const;

    std::string getChannel() { return _channel; };
    std::string getMsg() { return _msg; };

private:
    Topic();
    std::string     _channel;
    std::string     _msg;
};

#endif // TOPIC_HPP