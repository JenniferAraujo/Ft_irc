#ifndef JOIN_HPP
# define JOIN_HPP

# include "Includes.hpp"

# define BADCHANNELKEY 475

inline std::string JOIN_CHANNEL(const std::string& nick, const std::string& user, const std::string& host, const std::string& channel) {
    return ":" + nick + "!" + user + "@" + host + " JOIN " + channel + "\r\n";
}

class Join: public ACommand {
public:
    Join(Server& server, Client& client);
    ~Join() {};

    void parsing(std::istringstream &input);
    void parsingToken(std::string token, int n);
    void execute();
    void print() const;

    std::queue<std::string> getPassword() { return _password; };
    std::queue<std::string> getChannels() { return _channels; };

private:
    Join();
    std::queue<std::string> _channels;
    std::queue<std::string> _password;
};

#endif // JOIN_HPP