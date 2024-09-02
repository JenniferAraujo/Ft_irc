# include "Includes.hpp"

//? :<Nick>!<User>@<Host> PRIVMSG <target> :<Msg Content>
inline std::string PRIV_MESSAGE(const std::string& nick, const std::string& user, const std::string& host, const std::string& target, const std::string& msg) {
    return ":" + nick + "!" + user + "@" + host + " PRIVMSG "  + target + " :" + msg + "\r\n";
}