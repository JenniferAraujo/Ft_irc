#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Includes.hpp"

class Client;

class Channel {
public:
    Channel(std::string name);
    ~Channel(); // Destructor

    void addClient(Client &client);

private:
    Channel();
    std::string _name;
    std::map<int, Client*>   _Clients;

};

#endif // CHANNEL_HPP