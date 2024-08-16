#ifndef MACROS_HPP
# define MACROS_HPP

#include "Includes.hpp"

class Client;

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
inline std::string RPL_MYINFO(const std::string& client, const std::string& nick, const std::string& serverName) {
    return ":" + client + " 004 " + nick + " " + serverName + " <version> NCcCtu\r\n";
}
inline std::string RPL_ISUPPORT(const std::string& client, const std::string& nick) {
    return ":" + client + " 005 " + nick + " USERLEN=12 :are supported by this server\r\n";
}
inline std::string RPL_MOTDSTART(const std::string& client, const std::string& nick, const std::string& serverName) {
    return ":" + client + " 375 " + nick + " :" + serverName + " Message of the day\r\n";
}
inline std::string RPL_MOTD(const std::string& client, const std::string& msg, const std::string& nick) {
    return ":" + client + " 372 " + nick + " :" + msg + "\r\n";
}
inline std::string RPL_ENDOFMOTD(const std::string& client, const std::string& nick) {
    return ":" + client + " 376 " + nick + " :End of /MOTD command.\r\n";
}
inline std::string RPL_MODE(const std::string& client, const std::string& channel, const std::string& nick, const std::string& mode) {
    return ":" + client + " 324 " + nick + " #" + channel + " " + mode + "\r\n";
}
inline std::string RPL_WHO(const std::string& client, const std::string& channel, const std::string& nick, const Client& c) {
    return ":" + client + " 352 " + nick + " #" + channel + " " + c.getUsername() + " " + c.getIpaddr() + " " + client + " " + c.getNick() + " H :0 " + c.getRealname() + "\r\n";
}
inline std::string RPL_ENDWHO(const std::string& client, const std::string& channel, const std::string& nick) {
    return ":" + client + " 315 " + nick + " #" + channel + " :END of /WHO list.\r\n";
}
inline std::string RPL_NAME(const std::string& client, const std::string& channel, const std::string& nick, const std::string& nicks) {
    return ":" + client + " 353 " + nick + " = #" + channel + " :" + nicks + "\r\n";
}
inline std::string RPL_ENDNAME(const std::string& client, const std::string& channel, const std::string& nick) {
    return ":" + client + " 366 " + nick + " #" + channel + " :End of NAMES list\r\n";
}

//? SERVER MESSAGES
inline std::string ERROR(const std::string & msg) {
    return "ERROR :" + msg + "\r\n";
}

//? :<Nick>!<User>@<Host> PRIVMSG <Nick Intended>: <Msg Content>
inline std::string PRIV_MESSAGE(const std::string& nick, const std::string& user, const std::string& host, const std::string& nickIntended, const std::string& msg) {
    return ":" + nick + "!" + user + "@" + host + " PRIVMSG "  + nickIntended + ": " + msg + "\r\n";
}

#endif // MACROS_HPP
