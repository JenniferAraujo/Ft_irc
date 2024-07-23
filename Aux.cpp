#include "Includes.hpp"

std::ostream &operator<<(std::ostream &out, const std::vector<pollfd>::iterator &it) {
    out << "FD: " << BOLD_GREEN << it->fd <<
        RESET << " | Events: " << it->events
        << " | Revents: " << (it->revents == 0 ? BOLD_YELLOW : BOLD_RED) << it->revents
        << RESET << " |" << std::endl;
    return out;
}

std::ostream &operator<<(std::ostream &out, const pollfd &pfd)
{
    out << "FD: " << pfd.fd
        << " | Events: " << pfd.events
        << " | Revents: " << pfd.revents << " |" << std::endl;
    return out;
}

// Overload operator<< for vector of pollfd
std::ostream &operator<<(std::ostream &out, const std::vector<pollfd> &NFDs)
{
    for (std::vector<pollfd>::const_iterator it = NFDs.begin(); it != NFDs.end(); ++it)
    {
        out << *it; // Dereference the iterator to access pollfd
    }
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