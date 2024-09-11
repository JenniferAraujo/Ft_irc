#ifndef WHO_HPP
# define WHO_HPP

# include "Includes.hpp"

inline std::string RPL_WHO(const std::string& source, const std::string& channel, const std::string& target, const std::string& mode, const Client& c) {
    return ":" + source + " 352 " + target + " " + channel + " " + c.getUsername() + " " + c.getIpaddr() + " " + source + " " + c.getNick() + " H" + mode + " :0 " + c.getRealname() + "\r\n";
}
inline std::string RPL_ENDWHO(const std::string& source, const std::string& channel, const std::string& target) {
    return ":" + source + " 315 " + target + " " + channel + " :End of /WHO list.\r\n";
}
inline std::string RPL_NAME(const std::string& source, const std::string& channel, const std::string& target, const std::string& targets) {
    return ":" + source + " 353 " + target + " = " + channel + " :" + targets + "\r\n";
}
inline std::string RPL_ENDNAME(const std::string& source, const std::string& channel, const std::string& target) {
    return ":" + source + " 366 " + target + " " + channel + " :End of NAMES list\r\n";
}

class Who: public ACommand {
public:
    Who(Server& server, Client& client);
    ~Who() {};

    void parsing(std::istringstream &input);
    void execute();
    void print() const;

    std::string getChannel() { return _channel; };
private:
    Who();
    std::string _channel;

};


#endif // WHO_HPP