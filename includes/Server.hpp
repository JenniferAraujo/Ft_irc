#ifndef SERVER_HPP
# define SERVER_HPP

# include "Client.hpp"

class Server {
public:
    Server(); // Constructor
    Server(const Server& copy);//Copy constructor
    Server& operator= (const Server& copy); //Copy assignment constructor
    ~Server(); // Destructor

private:
    // Add private members here
};

#endif // SERVER_HPP
