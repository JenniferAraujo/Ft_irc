#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Includes.hpp"

class Client;
class Mode;

class Channel {
public:
	Channel(std::string name);
	~Channel(); // Destructor

	//!SECTION - Getters
	std::map<int, Client*>	getClients() const { return _Clients; } ;
	std::map<int, Client*>	getOperators() const { return _operators; } ;
	std::vector<int>		getInvited() const { return _invitedClients; } ;
	std::string				getMode() const { return _mode; }
	int						getUserLimit() const { return _userLimit; };
	const std::string&		getPassword() const { return _password; };
	std::string				getTopic() const { return _topic; } ;
	std::string				getName() const { return _name; } ;
	bool					getInviteOnly() const { return _inviteOnly; } ;
	bool					getTopicProtected() const { return _topicProtected; } ;

	//!SECTION - Setters
	void	setPassword(const std::string& password) { _password = password; }
	void	setUserLimit(int limit) { _userLimit = limit; }
	void	setTopic(std::string topic) {	_topic = topic; };

	//channel management
	void	applyMode(const Mode& modeObj, char modeChar, bool adding);
	bool	isInviteOnly() const { return _inviteOnly; }
	bool	isTopicLocked() const { return _topicProtected; }
	bool	isPasswordProtected() const { return !_password.empty(); }
	bool	isOperator(int clientId) const { return _operators.find(clientId) != _operators.end(); }
	int		canJoin(const Client& client, std::string password) const;
	void	addOperator(int clientId, Client* client);
	void	removeOperator(int clientId);
	//void	addClient(int clientId, Client* client) { _Clients[clientId] = client; }
	void	removeClient(int clientId);
	void	removeInvited(int clientId);
	void	addClient(Client &client);

	//aux mode
	bool	isInvited(const int& fd) const { return std::find(_invitedClients.begin(), _invitedClients.end(), fd) != _invitedClients.end(); }
	bool	hasPassword(const std::string& password) const { 
		if (password.empty())
			return false;
		return _password == password; };

	void	addInviteFD(const int fd) { _invitedClients.push_back(fd); }
	bool	isClient(int fd) { return this->_Clients.find(fd) != this->_Clients.end() ? true : false; };
	bool	isClient(int fd) const { return this->_Clients.find(fd) != this->_Clients.end() ? true : false; };
	bool	isOperator(int fd) { return this->_operators.find(fd) != this->_operators.end() ? true : false;	};
	void	sendMessage(std::string msg, int skipFD);
	void	sendMessageToOperators(std::string msg, int skipFD);
	void	sendMessageToClients(std::string msg, int skipFD);
	Client* getClientByNick(const std::string& nick);
	Client* getOperatorByNick(const std::string& nick);
	void	printOperators() const;

private:
	Channel();
	std::string _name;
	std::string _mode;
	std::string	_password;
	std::string	_topic;
	bool		_inviteOnly;
	bool		_topicProtected;
	int			_userLimit;
	std::map<int, Client*>   _Clients;
	std::map<int, Client*>	_operators; //NOTE - channel operators 
	//NOTE - Se o cliente desconectar tem que ser apagado de todas estas collections
	std::vector<int>			_invitedClients; //NOTE - clientes que foram convidados p entrar em canal - fd
};

#endif // CHANNEL_HPP