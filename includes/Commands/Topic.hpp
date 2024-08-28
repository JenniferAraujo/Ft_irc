#ifndef TOPIC_HPP
# define TOPIC_HPP

# include "Includes.hpp"

inline std::string RPL_NOTOPIC(const std::string& client, const std::string& channel, const std::string& nick) {
    return ":" + client + " 331 " + nick + " " + channel + " :No topic is set\r\n";
}

inline std::string RPL_TOPIC(const std::string& client, const std::string& channel, const std::string& nick, const std::string& topic) {
    return ":" + client + " 332 " + nick + " " + channel + " " + topic + "\r\n";
}

class Topic: public ACommand {
public:
    Topic(Server& server, Client& client);
    ~Topic() {};

    void parsing(std::istringstream &input);
    void execute();
    void print() const;

    std::string getChannel() { return _channel; };
    std::string getMsg() { return _msg; };
    bool getRemoveTopic() { return _removeTopic; };

private:
    Topic();
    std::string     _channel;
    std::string     _msg;
    bool            _removeTopic;
};

#endif // TOPIC_HPP