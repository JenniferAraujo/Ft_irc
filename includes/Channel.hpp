#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Includes.hpp"

class Client;

class Channel {
public:
    Channel(std::string name);
    ~Channel(); // Destructor

    void addClient(Client &client);

    std::map<int, Client*> getClients() const { return _Clients; } ;
	std::string getMode() const { return _mode; }
	void setMode(const std::string &mode) { _mode = mode; }

private:
    Channel();
    std::string _name;
	std::string _mode;
    std::map<int, Client*>   _Clients; //NOTE - sugestion: key ser uma bool - true se é operador
};

#endif // CHANNEL_HPP