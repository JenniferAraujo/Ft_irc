#ifndef ACOMMAND_HPP
# define ACOMMAND_HPP

# include "Includes.hpp"

class ACommand {
public:
    ACommand(std::vector<char> buf);
    virtual ~ACommand();

    //assim?
    virtual void parsing() = 0;

private:
    ACommand();

};

#endif // PARSER_HPP