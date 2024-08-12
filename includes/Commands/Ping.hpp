#ifndef PING_HPP
# define PING_HPP

# include "Includes.hpp"

inline std::string PONG(const std::string& server, const std::string& token) {
    return ":" + server + " PONG " + server + " :" + token + "\r\n";
}

class Ping: public ACommand {
public:
    Ping(Server& server, Client& client);
    ~Ping() {};

    void parsing(std::istringstream &input);
    void execute();

    std::string getToken() { return _token; };

private:
    Ping();
    std::string _token;
};

#endif // PING_HPP