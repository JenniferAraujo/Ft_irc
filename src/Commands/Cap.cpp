#include "Includes.hpp"

Cap::Cap(const Server& server, const Client& client): ACommand("CAP", server, client), _end(false){};

void Cap::parsing(std::istringstream &input){
	std::string str;
    std::getline(input, str, ' ');
    if(!str.compare("END\r"))
        this->_end = true;
    else if(str != "LS")
        this->_error = 1;
}
