#include "Includes.hpp"

Join::Join(Server& server, Client& client): ACommand("JOIN", server, client) {};

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
        this->trimChar(token, '\r'); //REVIEW - nao precisas disto aqui, é só quando les até ao \n
        std::cout << "Token: " << token << std::endl;
        int count = std::count(token.begin(), token.end(), ',');
        if (count == 0)
			this->parsingToken(token, n);
        else {
            std::istringstream tokenstream(token);
            while (std::getline(tokenstream, token, ',')) {
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

void Join::execute() {
    std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << this->_name << std::endl;
    std::string msg;
    msg.append(JOIN_CHANNEL(this->_client.getNick(), this->_client.getUsername(), this->_client.getIpaddr(), this->_channel));
    this->_server.addInChannel(this->_channel, const_cast<Client&>(this->_client));
    send(this->_client.getSocketFD(), msg.c_str(), msg.length(), 0);
}

void Join::print() const{
    std::cout << "Command: " << this->_name <<  " | Error: " << this->_error << " | Channel: " << this->_channel << std::endl;
}
