#ifndef MACROS_HPP
# define MACROS_HPP

#include "Includes.hpp"

//? :<Client> <Numeric Code> <Nick Intended>: <Msg Content>
inline std::string RPL_WELCOME(const std::string& client, const std::string& networkName, const std::string& nick, const std::string& user, const std::string& host) {
    return ":" + client + " 001 " + nick + " :Welcome to the " + networkName + " Network, " + nick + "!" + user + "@<" + host + ">\r\n";
}
inline std::string RPL_YOURHOST(const std::string& client, const std::string& serverName, const std::string& nick, const std::string& version) {
    return ":" + client + " 002 " + nick + " :Your host is <" + serverName + ">, running version <" + version + ">\r\n";
}
inline std::string RPL_CREATED(const std::string& client, const std::string& dateTime, const std::string& nick) {
    return ":" + client + " 003 " + nick + " :This server was created " + dateTime + "\r\n";
}
inline std::string RPL_MOTD(const std::string& client, const std::string& msg, const std::string& nick) {
    return ":" + client + " 372 " + nick + " :" + msg + "\r\n";
}

//? SERVER MESSAGES
inline std::string ERROR(const std::string & msg) {
    return "ERROR :" + msg + "\r\n";
}

//? :<Nick>!<User>@<Host> PRIVMSG <Nick Intended>: <Msg Content>
inline std::string PRIV_MESSAGE(const std::string& nick, const std::string& user, const std::string& host, const std::string& nickIntended, const std::string& msg) {
    return ":" + nick + "!" + user + "@" + host + " PRIVMSG "  + nickIntended + ": " + msg + "\r\n";
}

//? :<Nick>!<User>@<Host> JOIN #<Channel>
inline std::string JOIN_CHANNEL(const std::string& nick, const std::string& user, const std::string& host, const std::string& channel) {
    return ":" + nick + "!" + user + "@" + host + " JOIN #" + channel + "\r\n";
}

#endif // MACROS_HPP
