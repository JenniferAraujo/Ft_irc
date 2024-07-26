#ifndef INCLUDES_HPP
# define INCLUDES_HPP

# include <iostream>
# include <string.h>
# include <unistd.h>
# include <cstdlib>
# include <ctime>
# include <stdexcept>
# include <cctype>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <poll.h>
# include <vector>
# include <map>
# include <fstream>
# include <algorithm>
# include <fstream>
# include <sstream>
# define RED "\033[31m"
# define GREEN "\033[32m"
# define PURPLE "\033[35m"
# define YELLOW "\033[33m"
# define WHITE "\033[37m"
# define CYAN "\033[36m"
# define BOLD_RED "\033[1;31m"
# define BOLD_GREEN "\033[1;32m"
# define BOLD_PURPLE "\033[1;35m"
# define BOLD_YELLOW "\033[1;33m"
# define BOLD_WHITE 	"\033[1;37m"
# define BOLD_CYAN "\033[1;36m"
# define RESET "\033[0m"

# define INVALIDPASS 0
# define INVALIDNICK 1
# define INVALIDUSER 2

inline std::string RPL_WELCOME(const std::string& client, const std::string& networkName, const std::string& nick) {
    return ":" + client + " 001 " + nick + " :Welcome to the <" + networkName + "> Network, <" + nick + ">\r\n";
}

inline std::string RPL_YOURHOST(const std::string& client, const std::string& serverName, const std::string& nick, const std::string& version) {
    return ":" + client + " 002 " + nick + " :Your host is <" + serverName + ">, running version <" + version + ">\r\n";
}

inline std::string RPL_CREATED(const std::string& client, const std::string& dateTime, const std::string& nick) {
    return ":" + client + " 003 " + nick + " :This server was created <" + dateTime + ">\r\n";
}

inline std::string RPL_MOTD(const std::string& client, const std::string& msg, const std::string& nick) {
    return ":" + client + " 372 " + nick + " :" + msg + "\r\n";
}

inline std::string ERROR(const std::string & msg) {
    return "ERROR :" + msg + "\r\n";
}

# include "Client.hpp"
# include "Server.hpp"
# include "IRCException.hpp"


std::ostream &operator<<(std::ostream &out, const pollfd &pfd);
std::ostream &operator<<(std::ostream &out, const std::vector<pollfd> &NFDs);
std::ostream& operator<<(std::ostream& out, const Client &client);

template <typename K, typename V>
void printMap(const std::map<K, V>& map) {
    for (typename std::map<K, V>::const_iterator it = map.begin(); it != map.end(); ++it) {
        std::cout << GREEN << "[" << it->first << "]" << RESET << " => " << *it->second << std::endl;
    }
}

std::string getCurrentDateTime();

#endif // INCLUDES_HPP
