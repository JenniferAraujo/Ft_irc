#ifndef PART_HPP
# define PART_HPP

# include "Includes.hpp"

inline std::string PART(const std::string& nick, const std::string& user, const std::string& host, const std::string& channel, const std::string& message) {
    return ":" + nick + "!" + user + "@" + host + " PART " + channel + " :" + message + "\r\n";
}

class Part: public ACommand {
public:
    Part(Server& server, Client& client);
    ~Part() {};

    void parsing(std::istringstream &input);
    void execute();
    void print() const;

    void parsingToken(std::string token, int n,  std::istringstream &input);
    std::queue<std::string> getChannels() { return _channels; };
    std::string getMessage() { return _message; };

private:
    Part();
    std::queue<std::string> _channels;
    std::string             _message;
};

#endif // PART_HPP