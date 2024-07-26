#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Includes.hpp"

class Channel {
public:
    ~Channel(); // Destructor
    Channel(std::string name);

private:
    Channel();
    std::string _name;

};

#endif // CHANNEL_HPP