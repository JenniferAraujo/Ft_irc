#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include "Includes.hpp"

class Send{
public:
    Send(int fd, std::string msg, Server &server): _fd(fd), _msg(msg), _server(server) {
        this->sendMessage();
    };
    ~Send() {};

private:
    Send();
    void sendMessage(){
        try
        {
            if(send(this->_fd, _msg.c_str(), _msg.length(), 0) == -1){
                if (errno == ECONNRESET)
                    _server.updateToRemove(_fd, "");
                throw IRCException("[ERROR] Sending message to client with fd [" +  std::to_string(_fd) + "] went wrong: " + strerror(errno));
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what()  << std::endl;
        }

    };
    int          _fd;
    std::string  _msg;
    Server       _server;
};

#endif 