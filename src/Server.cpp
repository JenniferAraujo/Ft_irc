#include "Includes.hpp"

Server::Server()
{
}

Server::Server(const int &port, const std::string &password) : _port(port),
															   _password(password)
{
	memset(&this->_socketInfo, 0, sizeof(this->_socketInfo));
	_socketInfo.sin6_family = AF_INET6;
	_socketInfo.sin6_port = htons(this->_port);
	_socketInfo.sin6_addr = in6addr_any;
	this->_creationTime = getCurrentDateTime();
}

Server::Server(const Server &cpy): _port(cpy.getPort()), _password(cpy.getPassword())
{
}

// Função para adicionar fds para a poll monitorizar
void Server::updateNFDs(int fd)
{
	pollfd pfd = {fd, POLLIN, 0};
	this->_NFDs.push_back(pfd);
}

void Server::updateClients(Client *client, int fd)
{
	this->_Clients[fd] = client;
}

//Se o cliente ja tiver registado da erro
//Se a pasward for invalida da erro e seta a flag AuthError
//Se a pasward for valida seta a pass do cliente
void Server::pass(Client &client, ACommand *command) {
	std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << RESET << command->getName() << std::endl;
	std::string msg;
	if(client.getRegistration()){
		msg.append(ERROR("You may not reregister"));
		send(client.getSocketFD(), msg.c_str(), msg.length(), 0);
		return ;
	}
	Pass    *pass = dynamic_cast<Pass *>(command);
	//Isto tem que funcionar, nao pode ser nulo, mas vou por aqui a exception para debug
	if(pass == NULL)
		throw IRCException("[ERROR] pass dynamic cast went wrong");
	if (command->getError()) {
		msg.append(ERROR("Password incorrect"));
		send(client.getSocketFD(), msg.c_str(), msg.length(), 0);
		this->_toRemove.push_back(client.getSocketFD());
		client.setAuthError(PASSWDMISMATCH);
	}
	else
		client.setPass(pass->getPass());
}

void Server::join(Client &client, ACommand *command) {
	std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << command->getName() << std::endl;
	std::string msg;
	Join    *join = dynamic_cast<Join *>(command);
	if(join == NULL)
		throw IRCException("[ERROR] pass dynamic cast went wrong");
	msg.append(JOIN_CHANNEL(client.getNick(), client.getUsername(), client.getIpaddr(), join->getChannel()));
	this->addInChannel(join->getChannel(), const_cast<Client&>(client));
	send(client.getSocketFD(), msg.c_str(), msg.length(), 0);
}

void Server::who(Client &client, ACommand *command) {
	std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << command->getName() << std::endl;
	std::string msg, names;
	Who    *who = dynamic_cast<Who *>(command);
	if(who == NULL)
		throw IRCException("[ERROR] Who dynamic cast went wrong");
	if (this->_Channels.find(who->getChannel()) != this->_Channels.end()) {
		Channel* channel = this->_Channels[who->getChannel()];
		std::map<int, Client*> clients = channel->getClients();
		for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it) {
			Client* c = it->second;
			names.append(c->getNick());
			names.append(" ");
			msg = RPL_WHO(this->_hostName, who->getChannel(), client.getNick(), *c);
			send(client.getSocketFD(), msg.c_str(), msg.length(), 0);
		}
	}
	msg = RPL_ENDWHO(this->_hostName, who->getChannel(), client.getNick());
	send(client.getSocketFD(), msg.c_str(), msg.length(), 0);
	msg = RPL_NAME(this->_hostName, who->getChannel(), client.getNick(), names);
	send(client.getSocketFD(), msg.c_str(), msg.length(), 0);
	msg = RPL_ENDNAME(this->_hostName, who->getChannel(), client.getNick());
	send(client.getSocketFD(), msg.c_str(), msg.length(), 0);
	send(client.getSocketFD(), msg.c_str(), msg.length(), 0);
}

bool	isValidMode(char mode) {
    const char validModes[] = {'i', 't', 'k', 'o', 'l'};
    size_t numValidModes = sizeof(validModes) / sizeof(validModes[0]);
    for (size_t i = 0; i < numValidModes; ++i) {
        if (validModes[i] == mode) {
            return true;
        }
    }
    return false;
}

void Server::mode(Client &client, ACommand *command) {
	std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << command->getName() << std::endl;
	std::string msg;
	Mode *mode = dynamic_cast<Mode *>(command);
	if (mode == NULL)
		throw IRCException("[ERROR] Mode dynamic cast went wrong");
	std::string channel = mode->getChannel();
	std::string modeStr = mode->getMode();
	if (channel.empty()) {
		msg.append("Error(461): MODE Not enough parameters.\r\n");
		send(client.getSocketFD(), msg.c_str(), msg.length(), 0);
		return;
	}
	std::map<std::string, Channel*>::iterator it = this->_Channels.find(channel);
	if (it == this->_Channels.end()) {
		msg.append("Reply(403): No such channel " + channel + "\r\n");
		send(client.getSocketFD(), msg.c_str(), msg.length(), 0);
		return;
	}
	for (size_t i = 0; i < modeStr.length(); ++i) { //colocar os erros no MACRO
        if (!isValidMode(modeStr[i])) {
            msg.append("Error(472): ");
            msg.append(1, modeStr[i]);
            msg.append(" is not a recognised channel mode.\r\n");
            send(client.getSocketFD(), msg.c_str(), msg.length(), 0);
            return ;
        }
    }
}

void Server::nick(Client &client, ACommand *command) {
	std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << RESET << command->getName() << std::endl;
	std::string msg;
	Nick    *nick = dynamic_cast<Nick *>(command);
	//Isto tem que funcionar, nao pode ser nulo, mas vou por aqui a exception para debug
	if(nick == NULL)
		throw IRCException("[ERROR] nick dynamic cast went wrong");
	if (command->getError()) {
		msg.append(ERROR("Invalid Nick"));
		send(client.getSocketFD(), msg.c_str(), msg.length(), 0);
		this->_toRemove.push_back(client.getSocketFD());
		client.setAuthError(INVALIDNICK);
	}
	else
		client.setNick(nick->getNick());
}

void Server::user(Client &client, ACommand *command) {
	std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << RESET << command->getName() << std::endl;
	std::string msg;
	if(client.getRegistration()){
		msg.append(ERROR("You may not reregister"));
		send(client.getSocketFD(), msg.c_str(), msg.length(), 0);
		return ;
	}
	User    *user = dynamic_cast<User *>(command);
	//Isto tem que funcionar, nao pode ser nulo, mas vou por aqui a exception para debug
	if(user == NULL)
		throw IRCException("[ERROR] user dynamic cast went wrong");
	if (command->getError()) {
		msg.append(ERROR("userword incorrect"));
		send(client.getSocketFD(), msg.c_str(), msg.length(), 0);
		this->_toRemove.push_back(client.getSocketFD()); //TODO condiçao para nao haver repetidos (funçao para adicionar ao toRemove)
		client.setAuthError(INVALIDUSER);
	}
	else{
		client.setUsername(user->getUsername());
		client.setRealname(user->getRealname());
	}
}

void Server::cap(Client &client, ACommand *command) {
	std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << command->getName() << std::endl;
	std::string msg;
	if(client.getRegistration()){ //NOTE Verificar isto
		msg.append(ERROR("You may not reregister"));
		send(client.getSocketFD(), msg.c_str(), msg.length(), 0);
		return ;
	}
	Cap    *cap = dynamic_cast<Cap *>(command);
	if(cap == NULL)
		throw IRCException("[ERROR] pass dynamic cast went wrong");
	(void)client;
}

void Server::welcome(Client &client) {
	std::cout << formatServerMessage(BOLD_WHITE, "CMD   ", 0) << RESET << "welcome" << std::endl;
	std::string msg;
	if (client.getPassword().empty()) {
		msg.append(ERROR("No password was set"));
		send(client.getSocketFD(), msg.c_str(), msg.length(), 0);
		this->_toRemove.push_back(client.getSocketFD());
	} else {
		msg.append(RPL_WELCOME(this->_hostName, "Internet Fight Club", client.getNick(), client.getUsername(), client.getIpaddr()));
		msg.append(RPL_YOURHOST(this->_hostName, "servername", client.getNick(), "version"));
		msg.append(RPL_CREATED(this->_hostName, this->getCreationTime(), client.getNick()));
		msg.append(RPL_MOTD(this->_hostName, "  ________________", client.getNick()));
		msg.append(RPL_MOTD(this->_hostName, " /______________ /|", client.getNick()));
		msg.append(RPL_MOTD(this->_hostName, "|  ___________  | |", client.getNick()));
		msg.append(RPL_MOTD(this->_hostName, "| |           | | |", client.getNick()));
		msg.append(RPL_MOTD(this->_hostName, "| |  ft_irc   | | |", client.getNick()));
		msg.append(RPL_MOTD(this->_hostName, "| |    by:    | | |", client.getNick()));
		msg.append(RPL_MOTD(this->_hostName, "| |           | | |", client.getNick()));
		msg.append(RPL_MOTD(this->_hostName, "| |  r, j, d  | | |", client.getNick()));
		msg.append(RPL_MOTD(this->_hostName, "| |___________| | |  ___", client.getNick()));
		msg.append(RPL_MOTD(this->_hostName, "|_______________|/  /  /", client.getNick()));
		msg.append(RPL_MOTD(this->_hostName, "                   /__/", client.getNick()));
		send(client.getSocketFD(), msg.c_str(), msg.length(), 0);
	}
}

//TODO comando cap
//TODO comando ping

//*Proximos passos: canais e mensagens
//Ideia para executar os cmds
void    Server::executeCommand(Client &client, ACommand *command){
	int N = 7;
	std::string commands[] = {"CAP", "PASS", "NICK", "USER", "JOIN", "MODE", "WHO"};
	void (Server::*p[])(Client&, ACommand *) = {&Server::cap, &Server::pass, &Server::nick, &Server::user, &Server::join, &Server::mode, &Server::who};
	for (int i = 0; i < N; i++) {
		if(!commands[i].compare(command->getName())){
			if((!client.getRegistration() || client.getAuthError()) && !registration_command(command->getName()))
				std::cout << RED << "ERROR: " << RESET
				<< "Auth not over or auth error -> client can not execute command: " <<
				command->getName() << ", and will be disconected soon" << std::endl;
			else
				(this->*p[i])(client, command);
		}
	}
}

//QUIT -> fechar o fd
// Função para verificar eventos na poll
//Verificar it->revents == POLLIN
//Verificar it-> revents == POLLOUT
//if _passward = false -> dar handle de alguma forma?
void Server::verifyEvent(const pollfd &pfd) {
	if(pfd.revents == POLLIN) {
		Client *client = this->_Clients[pfd.fd];
		//printMap(this->_Clients);
		//TODO disconnection depending on cap
		/*if(client.capEnd == false && client.ping > 5){
			this->_toRemove.push_back(client.getSocketFD());
			client->setAuthError(INVALIDCAP);
		} */
		std::vector<char> buf(5000);
		recv(client->getSocketFD(), buf.data(), buf.size(), 0);
		// std::cout << buf.data() << "." << std::endl;
		//Setting pass, nick, user --> n pode settar a pass se ja existir
		std::cout << formatServerMessage(BOLD_CYAN, "SERVER", this->_Clients.size()) << "Event on Client " << GREEN << "[" << client->getSocketFD() << "]" << RESET <<  std::endl;
		std::cout << formatServerMessage(BOLD_PURPLE, "C.INFO", 0) << *client << std::endl;
		std::queue<ACommand *> commands = client->createCommand(buf);
		if (commands.empty()) //Nao e um comando/ comando que nao tratamos
			return ;
		std::cout << BOLD_GREEN << "[PRINT COMMANDS]\n" << RESET;
		showq(commands);
		while(!commands.empty()){
			ACommand *command = commands.front();
			this->executeCommand(*client, command);
			commands.pop();
			delete command;
		}
		std::cout << BOLD_GREEN << "[PRINT CLIENTS]\n" << RESET;
		printMap(this->_Clients);
		if(!client->getRegistration() && !client->getAuthError() && !client->getNick().empty()
			&& !client->getRealname().empty() && !client->getUsername().empty()){
				//TODO welcome dependent on cap
				//if(client.cap == false || (client.cap == true && client.capEnd == true)){
					this->welcome(*client);
					client->setRegistration(true);
		}
		std::cout << std::endl;
	}
}

// Função para verificar que evento aconteceu
void Server::checkEvents(int nEvents) {
	(void)nEvents;
	std::vector<pollfd> NFDs2 = this->_NFDs;
	//titleInfo("Fds Vector");
	//std::cout << NFDs2;
	for (std::vector<pollfd>::iterator it = NFDs2.begin(); it != NFDs2.end(); ++it)
	{
		(it->fd == this->_socketFD)
			? Client::verifyConnection(*this, *it)
			: this->verifyEvent(*it);
	}
	//for (std::vector<int>::iterator it = toRemove.begin(); it != toRemove.end(); ++it) {
	//    this->removeClient(*it);
	//}
}

void Server::run()
{
	// Inicializa a socket e da exception se der erro
	this->_socketFD = socket(AF_INET6, SOCK_STREAM, 0);
	if (this->_socketFD == -1)
		throw IRCException("[ERROR] Opening socket went wrong");
	int enable = 1;
	if (setsockopt(this->_socketFD, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) == -1){
		close(this->_socketFD);
		throw IRCException("[ERROR] Setting socket options went wrong");
	}
	// Dá bind aquele mesma socket numa porta específica
	if (bind(this->_socketFD, reinterpret_cast<struct sockaddr *>(&this->_socketInfo), sizeof(this->_socketInfo)) == -1)
	{
		close(this->_socketFD);
		throw IRCException("[ERROR] Binding socket went wrong");
	}
	std::cout << BOLD_YELLOW << "[SERVER INFO]\t" << RESET << "Socket with fd " << GREEN "[" << this->_socketFD << "]" << RESET
			  << " bound on port " << YELLOW << this->_port << RESET << std::endl;
	// Meter a socket a ouvir aquela porta para um máximo de X conecções
	if (listen(this->_socketFD, 10) == -1)
	{
		close(this->_socketFD);
		throw IRCException("[ERROR] Listening socket went wrong");
	}
	std::cout << BOLD_YELLOW << "[SERVER INFO]\t" << RESET << "Server listening only " << YELLOW << 10 << RESET << " connections"
			  << std::endl;
	this->getServerInfo();
	// Adicionar o FD da socket aqueles que a poll vai poder monitorizar
	this->updateNFDs(this->_socketFD);
	// Ciclo para correr a poll para esperar eventos
	while (true)
	{
		switch (int nEvents = poll(this->_NFDs.data(), this->_NFDs.size(), -1))
		{
		case -1:
			throw IRCException("[ERROR] Poll went wrong");
		case 0:
			throw IRCException("[ERROR] Poll connection timed out");
		default:
			this->checkEvents(nEvents);
			//titleInfo("Clients Map");
			//printMap(_Clients);
			break;
		}
	}
	close(this->_socketFD);
}

Server::~Server() {
	for (std::map<int, Client*>::iterator it = _Clients.begin(); it != _Clients.end(); ++it) {
		close(it->first);
		delete it->second;
	}
}
