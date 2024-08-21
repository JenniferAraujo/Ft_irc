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
# include <string>
# include <cstring>
# include <fstream>
# include <algorithm>
# include <fstream>
# include <sstream>
# include <queue>
# include "IRCException.hpp"
# include "Server.hpp"
# include "Client.hpp"
# include "Macros.hpp"
# include "Channel.hpp"
# include "ACommand.hpp"
# include "Commands/Pass.hpp"
# include "Commands/Nick.hpp"
# include "Commands/User.hpp"
# include "Commands/Join.hpp"
# include "Commands/Part.hpp"
# include "Commands/Who.hpp"
# include "Commands/Mode.hpp"
# include "Commands/Cap.hpp"
# include "Commands/Ping.hpp"
# include "Commands/Kick.hpp"
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

# define NEEDMOREPARAMS 461
# define PASSWDMISMATCH 464

# define INVALIDNICK 2
# define INVALIDUSER 3
# define INVALIDCAP  4

# define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

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
void titleInfo(std::string title);
std::string intToString(int value);
std::string formatServerMessage(const std::string& color, const std::string& label, int clients);
void showq(std::queue<ACommand *> gq);
void showstringq(std::queue<std::string> gq);
void showdoublestringq(std::queue<std::string> gq, std::queue<std::string> gq2);

#endif // INCLUDES_HPP
