#include "Includes.hpp"
#include "Server.hpp"

void Message::sendMessage(int fd, std::string msg, Server &server){
    try {
        //std::cout << "Msg: fd: " << fd << " | msg: " << msg << std::endl;
        if(send(fd, msg.c_str(), msg.length(), 0) == -1){
           // std::cout << "Entra no error"<< std::endl;
            if (errno == ECONNRESET){
                //std::cout << "Entra no ECONNRESET"<< std::endl;
                server.updateToRemove(fd, "Send fail");
            }
            std::string error = strerror(errno);
            throw IRCException("[ERROR] Sending message to client[" + intToString(fd) + "went wrong: " + error);
        }
    }
    catch(const std::exception& e) {
        std::cerr << RED << e.what() << RESET << std::endl;
    }
};