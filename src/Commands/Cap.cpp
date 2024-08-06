#include "Includes.hpp"

Cap::Cap(const Server& server, const Client& client): ACommand("Cap", server, client) {};

void Cap::parsing(std::istringstream &input){
	std::string teste;
    std::getline(input, teste, '\r');
    std::cout << teste << std::endl;
}
