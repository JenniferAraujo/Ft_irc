#ifndef PARSER_HPP
# define PARSER_HPP

# include "Includes.hpp"

class Parser {
public:
    Parser(std::vector<char> buf);
    virtual ~Parser();

    //assim?
    virtual void treatCommand() = 0;

private:
    Parser();

};

#endif // PARSER_HPP