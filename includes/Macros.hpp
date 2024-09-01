#ifndef MACROS_HPP
# define MACROS_HPP

# include "Includes.hpp"

//? :<Client> <Numeric Code> <Nick Intended>: <Msg Content>
inline std::string RPL_WELCOME(const std::string& source, const std::string& networkName, const std::string& nick, const std::string& user, const std::string& host) {
    return ":" + source + " 001 " + nick + " :Welcome to the " + networkName + " Network, " + nick + "!" + user + "@" + host + "\r\n";
}
inline std::string RPL_YOURHOST(const std::string& source, const std::string& serverName, const std::string& target, const std::string& version) {
    return ":" + source + " 002 " + target + " :Your host is " + serverName + ", running version " + version + "\r\n";
}
inline std::string RPL_CREATED(const std::string& source, const std::string& dateTime, const std::string& target) {
    return ":" + source + " 003 " + target + " :This server was created " + dateTime + "\r\n";
}
inline std::string RPL_MYINFO(const std::string& source, const std::string& target, const std::string& serverName, const std::string& version) {
    return ":" + source + " 004 " + target + " " + serverName + " " + version + " NCcCtu\r\n";
}
inline std::string RPL_ISUPPORT(const std::string& source, const std::string& target) {
    std::stringstream ss;
    ss << ":" << source << " 005 " << target << " USERLEN=" << USERLEN << " :are supported by this server\r\n";
    return ss.str();
}
inline std::string RPL_MOTDSTART(const std::string& source, const std::string& target, const std::string& serverName) {
    return ":" + source + " 375 " + target + " :" + serverName + " Message of the day\r\n";
}
inline std::string RPL_MOTD(const std::string& source, const std::string& msg, const std::string& target) {
    return ":" + source + " 372 " + target + " :" + msg + "\r\n";
}
inline std::string RPL_ENDOFMOTD(const std::string& source, const std::string& target) {
    return ":" + source + " 376 " + target + " :End of /MOTD command.\r\n";
}
inline std::string RPL_MODE(const std::string& source, const std::string& channel, const std::string& target, const std::string& mode) {
    return ":" + source + " 324 " + target + " " + channel + " " + mode + "\r\n";
}
inline std::string RPL_WHO(const std::string& source, const std::string& channel, const std::string& target, const Client& c) {
    return ":" + source + " 352 " + target + " " + channel + " " + c.getUsername() + " " + c.getIpaddr() + " " + source + " " + c.getNick() + " H :0 " + c.getRealname() + "\r\n";
}
inline std::string RPL_ENDWHO(const std::string& source, const std::string& channel, const std::string& target) {
    return ":" + source + " 315 " + target + " " + channel + " :END of /WHO list.\r\n";
}
inline std::string RPL_NAME(const std::string& source, const std::string& channel, const std::string& target, const std::string& targets) {
    return ":" + source + " 353 " + target + " = " + channel + " :" + targets + "\r\n";
}
inline std::string RPL_ENDNAME(const std::string& source, const std::string& channel, const std::string& target) {
    return ":" + source + " 366 " + target + " " + channel + " :End of NAMES list\r\n";
}

//? SERVER MESSAGES
inline std::string ERROR(const std::string & msg) {
    return "ERROR :" + msg + "\r\n";
}

//? :<Nick>!<User>@<Host> PRIVMSG <target> :<Msg Content>
inline std::string PRIV_MESSAGE(const std::string& nick, const std::string& user, const std::string& host, const std::string& target, const std::string& msg) {
    return ":" + nick + "!" + user + "@" + host + " PRIVMSG "  + target + " :" + msg + "\r\n";
}

#endif // MACROS_HPP
