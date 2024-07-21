#include "Includes.hpp"

std::ostream &operator<<(std::ostream &out, const std::vector<pollfd>::iterator &it)
{
    out << "FD: " << it->fd
    << "\nEvents: " << it->events
    << "\nRevents: " << it->revents << std::endl;
    return out;
}