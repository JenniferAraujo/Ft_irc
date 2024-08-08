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

void    printCap(ACommand *command){
    Cap    *cap = dynamic_cast<Cap *>(command);
    std::cout << "Command: " << cap->getName() <<  " | Error: " << cap->getError() << " | End: " << cap->getEnd() << std::endl;

}

void    printPass(ACommand *command){
    Pass    *pass = dynamic_cast<Pass *>(command);
    std::cout << "Command: " << pass->getName() <<  " | Error: " << pass->getError() << " | Pass: " << pass->getPass() << std::endl;

}

void    printNick(ACommand *command){
    Nick    *nick = dynamic_cast<Nick *>(command);
    std::cout << "Command: " << nick->getName() <<  " | Error: " << nick->getError() << " | Nick: " << nick->getNick() << std::endl;

}

void    printUser(ACommand *command){
    User    *user = dynamic_cast<User *>(command);
    std::cout << "Command: " << user->getName() <<  " | Error: " << user->getError() << " | Name: " << user->getName() << " | Real name: " << user->getRealname() << std::endl;

}

void    printJoin(ACommand *command){
    Join    *join = dynamic_cast<Join *>(command);
    std::cout << "Name: " << join->getName() <<  " | Error: " << join->getError() << " | Channel: " << join->getChannel() << std::endl;

}

void    printMode(ACommand *command){
    Mode    *mode = dynamic_cast<Mode *>(command);
    std::cout << "Name: " << mode->getName() <<  " | Error: " << mode->getError() << " | Channel: " << mode->getChannel() << std::endl;

}

void    printWho(ACommand *command){
    Who    *who = dynamic_cast<Who *>(command);
    std::cout << "Name: " << who->getName() <<  " | Error: " << who->getError() << " | Channel: " << who->getChannel() << std::endl;

}

void    printPing(ACommand *command){
    Ping    *ping = dynamic_cast<Ping *>(command);
    std::cout << "Name: " << ping->getName() <<  " | Token: " << ping->getToken() << std::endl;

}

void printCommand(ACommand *command)
{
    int N = 8;
    std::string commands[] = {"CAP", "PASS", "NICK", "USER", "JOIN", "MODE", "WHO", "PING"};
    void (*p[])(ACommand *) = {printCap, printPass, printNick, printUser, printJoin, printMode, printWho, printPing};
    for (int i = 0; i < N; i++) {
        if(!commands[i].compare(command->getName())){
            (*p[i])(command);
        }
    }
}

void showq(std::queue<ACommand *> gq)
{
    std::queue<ACommand *> g = gq;
    if(g.empty())
        std::cout << "Ups, I'm empty!!\n";
    while (!g.empty()) {
        //std::cout << g.front()->getName() << std::endl;
        printCommand(g.front());
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