#ifndef PART_HPP
# define PART_HPP

# include "Includes.hpp"

class Part: public ACommand {
public:
    Part(Server& server, Client& client);
    ~Part() {};

    void parsing(std::istringstream &input);
    void execute();
    void print() const;

    void parsingToken(std::string token);
    std::queue<std::string> getChannels() { return _channels; };

private:
    Part();
    std::queue<std::string> _channels;
};

#endif // PART_HPP