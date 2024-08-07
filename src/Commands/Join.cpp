#include "Includes.hpp"

Join::Join(const Server& server, const Client& client): ACommand("JOIN", server, client) {};

void Join::parsingToken(std::string token, int n) {
	if (n == 0) {
        if (token[0] == '#') { //TODO - Mudar a lógica para usar as queue
            this->_channel = token;			 //JOIN #a
            this->_channels.push(token);	 //JOIN #a //JOIN #a,#b
        }
    	else
    	    this->_error = BADCHANNELKEY;	 //JOIN a //JOIN a,#b //JOIN #a,b
	} else {
		if (token[0] == '\0' || token[0] == '#')
			return;						 	 //JOIN #a, #b //JOIN #a ,#b
		else
			this->_password.push(token);	 //JOIN #a 123 //JOIN #a,#b 123,312
	}
}

void Join::parsing(std::istringstream &input){
	//? JOIN #a 123 a		 -> Verificar oque acontece
	std::string token;
	int n = 0;
    while (std::getline(input, token, ' ') && n < 2) {
        this->trimChar(token, '\r');
        std::cout << "Token: " << token << std::endl;
        int count = std::count(token.begin(), token.end(), ',');
        if (count == 0) {
			this->parsingToken(token, n);
        } else {
            std::istringstream tokenstream(token);
            while (std::getline(tokenstream, token, ',')) {
                std::cout << "[,] token: " << token << std::endl;
				this->parsingToken(token, n);
            }
        }
		n++;
    }
	// caso para se receber apenas um [JOIN], verificar se efetivamente entra aqui
	if (this->_channels.empty())
		this->_error = NEEDMOREPARAMS;
	std::cout << "_Channels queue: \n";
	showstringq(this->_channels);
	std::cout << "_Passwords queue: \n";
	showstringq(this->_password);
}
