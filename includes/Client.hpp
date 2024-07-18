#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Exception.hpp"

class Client {
public:
    Client(); // Constructor
    Client(const Client& copy);//Copy constructor
    Client& operator= (const Client& copy); //Copy assignment constructor
    ~Client(); // Destructor

private:
    // Add private members here
};

#endif // CLIENT_HPP
