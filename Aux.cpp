#include "Includes.hpp"

std::ostream &operator<<(std::ostream &out, const std::vector<pollfd>::iterator &it)
{
    out << "FD: " << it->fd
    << "\nEvents: " << it->events
    << "\nRevents: " << it->revents << std::endl;
    return out;
}

std::ostream& operator<<(std::ostream& out, const Client& client)
{
    out << "Fd " << client.getSocketFD()
    << "\nNick: " << client.getNick()
    << "\nName: " << client.getName()
    << "\nReal Name: " << client.getRealName() << std::endl;
    return(out);
}