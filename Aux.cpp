#include "Includes.hpp"

std::ostream &operator<<(std::ostream &out, const pollfd &pfd)
{
    out << "FD: " << BOLD_GREEN << pfd.fd <<
        RESET << " | Events: " << pfd.events
        << " | Revents: " << (pfd.revents == 0 ? BOLD_YELLOW : BOLD_RED) << pfd.revents
        << RESET << " |" << std::endl;
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

/*     int                     _socketFD;
    bool                    _validCmd;
    std::string             _nick;
    std::string             _name;
    std::string             _user;
    std::string             _realName;
    std::string             _command; */
//O nick e o user só imprime com \n e eu n percebo porque \n
std::ostream& operator<<(std::ostream& out, const Client &client)
{
    out << "FD: " << BOLD_GREEN << client.getSocketFD() << RESET
    << " | Command: " << client.getCommand()
    << " | Nick: " << client.getNick()
    << " | Username: " << client.getUsername()
    << " | Realname: " << client.getRealname() << std::endl;
    return(out);
}

std::string getCurrentDateTime()
{
	char buffer[80];
	std::time_t rawtime;
	std::tm *timeinfo;
	std::time(&rawtime);
	timeinfo = std::localtime(&rawtime);
	std::strftime(buffer, 80, "%H:%M:%S %b %d %Y", timeinfo);
	return std::string(buffer);
}