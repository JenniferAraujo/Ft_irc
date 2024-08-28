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
    << "\t| Nick: " << client.getNick()
    << " | Username: " << client.getUsername()
    << " | Realname: " << client.getRealname();
    return(out);
}

void titleInfo(std::string title) {
    std::cout << BOLD_YELLOW << "[" << title << "]" << RESET << std::endl;
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

std::string getCurrentTime()
{
    char buffer[80];
    std::time_t rawtime;
    std::tm *timeinfo;
    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
    std::strftime(buffer, 80, "%H:%M:%S", timeinfo);
    return std::string(buffer);
}

std::string intToString(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
};

std::string formatServerMessage(const std::string& color, const std::string& label, int clients) {
    std::ostringstream oss;
    if (clients == 0)
        oss << color << "[" << label << "]" << RESET << "[" << getCurrentTime() << "]\t";
    else
        oss << color << "[" << label << "]" << RESET << "[" << getCurrentTime() << "][ " << clients << " ] ";
    return oss.str();
}

void showq(std::queue<ACommand *> gq)
{
    std::queue<ACommand *> g = gq;
    if(g.empty())
        std::cout << "Ups, I'm empty!!\n";
    while (!g.empty()) {
        //std::cout << g.front()->getName() << std::endl;
        g.front()->print();
        g.pop();
    }
    std::cout << '\n';
}

void showstringq(std::queue<std::string> gq)
{
    std::queue<std::string> g = gq;
    if(g.empty())
        std::cout << "Ups, I'm empty!!\n";
    while (!g.empty()) {
        std::cout << g.front() << std::endl;
        g.pop();
    }
}

void showdoublestringq(std::queue<std::string> gq, std::queue<std::string> gq2)
{
    std::queue<std::string> g = gq;
    std::queue<std::string> g2 = gq2;
    while (!g.empty()) {
        std::cout << g.front() << "\t\t| ";
        g.pop();
        if (!g2.empty()) {
            std::cout << g2.front();
            g2.pop();
        }
        std::cout << std::endl;
    }
}