#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Includes.hpp"

class Client;
class Mode;

class Channel {
public:
	Channel(std::string name);
	~Channel(); // Destructor

	void	addClient(Client &client);
	
	std::map<int, Client*> getClients() const { return _Clients; } ;
	
	std::string getMode() const { return _mode; }
	void	setMode(const std::string &mode) { _mode = mode; }

	//gerenciamento do canal
	void	applyMode(const Mode& modeObj);
	bool	isInviteOnly() const { return _inviteOnly; }
	bool	isTopicLocked() const { return _topicProtected; }
	int		getUserLimit() const { return _userLimit; }
	Client*	getClientById(int id) const;
	const	std::string& getPassword() const { return _password; }
	bool	isPasswordProtected() const { return !_password.empty(); }
	bool	isOperator(int clientId) const { return _operators.find(clientId) != _operators.end(); }
	bool	canJoin(const Client& client) const;

	void	setPassword(const std::string& password) { _password = password; }
	void	setUserLimit(int limit) { _userLimit = limit; }
	void	addOperator(int clientId, Client* client) { _operators.insert(std::make_pair(clientId, client)); }
	void	removeOperator(int clientId) { _operators.erase(clientId); }
	void	addClient(int clientId, Client* client) { _Clients[clientId] = client; }
	
private:
	Channel();
	std::string _name;
	std::string _mode;
	std::string	_password;
	bool		_inviteOnly;
	bool		_topicProtected;
	int			_userLimit;
	std::map<int, Client*>   _Clients; //NOTE - sugestion: key ser uma bool - true se é operador
	std::map<int, Client*>	_operators; //NOTE - channel operators 
};

#endif // CHANNEL_HPP