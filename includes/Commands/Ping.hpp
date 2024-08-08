#ifndef PING_HPP
# define PING_HPP

# include "Includes.hpp"

inline std::string PONG(const std::string& server, const std::string& token) {
    return ":" + server + " PONG " + server + " :" + token + "\r\n";
}

class Ping: public ACommand {
public:
    Ping(const Server& server, const Client& client);
    ~Ping() {};

    void parsing(std::istringstream &input);

    std::string getToken() { return _token; };

private:
    Ping();
    std::string _token;
};

#endif // PING_HPP