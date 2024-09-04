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
	int						getUserLimit() const { return _userLimit; }
	const std::string&		getPassword() const { return _password; }
    std::string				getTopic() const { return _topic; } ;
    std::string				getName() const { return _name; } ;
	Client*					getClientById(int id) const;

	//!SECTION - Setters
	void	setMode(const std::string &mode) { _mode = mode; }
	void	setPassword(const std::string& password) { _password = password; }
	void	setUserLimit(int limit) { _userLimit = limit; }
    void	setTopic(std::string topic) { _topic = topic; } ;
	//? 	JENNY???????
	void	setEnteredPassword(const std::string& password) { _password = password; }

	//channel management
	void	applyMode(const Mode& modeObj);
	bool	isInviteOnly() const { return _inviteOnly; }
	bool	isTopicLocked() const { return _topicProtected; }
	bool	isPasswordProtected() const { return !_password.empty(); }
	bool	isOperator(int clientId) const { return _operators.find(clientId) != _operators.end(); }
	bool	canJoin(const Client& client, std::string password) const;
	void	addOperator(int clientId, Client* client);
	void	removeOperator(int clientId);
	void	addClient(int clientId, Client* client) { _Clients[clientId] = client; }
	void	removeClient(int clientId);
	void	removeInvited(int clientId);
	void	addClient(Client &client);

	//aux mode
	bool	isInvited(const std::string& name) const { return std::find(_invitedNames.begin(), _invitedNames.end(), name) != _invitedNames.end(); }
	void	addInvite(const std::string& name) { _invitedNames.push_back(name); }
	void	addInviteFD(const int fd) { _invitedClients.push_back(fd); }
	bool	hasPassword(const std::string& password) const { return password.empty() ? false : _password == password; };
	bool	isClient(int fd) { return this->_Clients.find(fd) != this->_Clients.end() ? true : false; };
	bool	isOperator(int fd) { return this->_operators.find(fd) != this->_operators.end() ? true : false;	};
	void	sendMessage(std::string msg, int skipFD);
	void	sendMessageToOperators(std::string msg, int skipFD);
	void	sendMessageToClients(std::string msg, int skipFD);

private:
	Channel();
	std::string _name;
	std::string _mode;
	std::string	_password;
	std::string	_topic;
	bool		_inviteOnly;
	bool		_topicProtected;
	int			_userLimit;
	//NOTE - Se o cliente desconectar tem que ser apagado de todas estas collections
	std::vector<std::string>	_invitedNames; //NOTE - clientes que foram convidados p entrar em canal
	std::vector<int>			_invitedClients; //NOTE - clientes que foram convidados p entrar em canal - fd
	std::map<int, Client*>		_Clients;
	std::map<int, Client*>		_operators; //NOTE - channel operators
};

#endif // CHANNEL_HPP