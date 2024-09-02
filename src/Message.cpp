#include "Includes.hpp"
#include "Server.hpp"

void Message::sendMessage(int fd, std::string msg, Server &server){
    try {
        if(send(fd, msg.c_str(), msg.length(), 0) == -1){ //limite de char enviados?
            if (errno == ECONNRESET)
                server.updateToRemove(fd, "");
            std::string error = strerror(errno);
            throw IRCException("[ERROR] Sending message to client[" + intToString(fd) + "went wrong: " + error);
        }
    }
    catch(const std::exception& e) {
        std::cerr << RED << e.what() << RESET << std::endl;
    }
};