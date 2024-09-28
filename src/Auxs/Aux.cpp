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

std::ostream& operator<<(std::ostream& out, const Client &client)
{
    out << "FD: " << BOLD_GREEN << client.getSocketFD() << RESET
    << "\t| Nick: " << client.getNick()
    << " | Username: " << client.getUsername()
    << " | Realname: " << client.getRealname()
    << " | Password: " << client.getPassword()
    << " | Reg Error: " << client.getRegError()
    << " | Registration bool: " << client.getRegistration();
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

std::string formatServerMessage(const std::string& color, const std::string& label, int clients, const std::string& fdColor) {
    std::ostringstream oss;
    if(clients == 0)
        oss << color << "[" << label << "]" << RESET << "[" << getCurrentTime() << "]\t";
    else if (fdColor.empty())
        oss << color << "[" << label << "]" << RESET << "[" << getCurrentTime() << "][ " << clients << " ]\t";
    else
        oss << color << "[" << label << "]" << RESET << "[" << getCurrentTime() << "]" << fdColor << "[ " << clients << " ]\t" << RESET;
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
        std::cout << "-";
    while (!g.empty()) {
        std::cout << g.front();
        g.pop();
        if (!g.empty())
            std::cout << ", ";
    }
}

void showMap(std::map<int, Client*> m) {
    if (m.empty()) {
        std::cout << "-";
        return;
    }
    std::map<int, Client*>::iterator it = m.begin();
    while (it != m.end()) {
        std::cout << it->second->getNick();
        ++it;
        if (it != m.end())
            std::cout << ", ";
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

void trimChar(std::string& str, char ch) {
    std::string::size_type first = str.find_first_not_of(ch);
    std::string::size_type last = str.find_last_not_of(ch);
    if (first == std::string::npos)
        str.clear();
    else
        str = str.substr(first, last - first + 1);
}

void signalHandler(int signal) {
    if (Server::_instance != NULL)
        Server::_instance->stopCompilation(signal);
}

// Function to convert a string to lowercase
std::string toLowerCase(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

bool  hasNonNumericChar(std::string str) {
  for (std::string::const_iterator i = str.begin(); i != str.end(); ++i) {
        if (!std::isdigit(*i))
            return true;
  }
  return false;
}

bool isInt(std::string str){
    if(str.empty())
        return false;
    if(hasNonNumericChar(str))
        return false;
    long long int lnum = std::atoll(str.c_str());
    if(lnum < INT_MIN || lnum > INT_MAX)
        return false;
    return true;
}
