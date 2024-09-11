#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include "Includes.hpp"

class Server;

class Message{
public:
    static void sendMessage(int fd, std::string msg, Server &server);

private:
    Message();
};

#endif 