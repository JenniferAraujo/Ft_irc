#ifndef CAP_HPP
# define CAP_HPP

# include "Includes.hpp"

class Parser;

class CAP: public Parser {
public:
    CAP();
    virtual ~CAP();

    //assim?
    void treatCommand();

private:
    /*
    int                     _authError;
    bool                    _password;
    bool                    _authOver;
    bool                    _validCmd;
    */
};

#endif // CAP_HPP