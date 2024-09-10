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

	//channel management
	void	applyMode(const Mode& modeObj);
	bool	isInviteOnly() const { return _inviteOnly; }
	bool	isTopicLocked() const { return _topicProtected; }
	int		getUserLimit() const { return _userLimit; }
	Client*	getClientById(int id) const;
	const	std::string& getPassword() const { return _password; }
	bool	isPasswordProtected() const { return !_password.empty(); }
	bool	isOperator(int clientId) const { return _operators.find(clientId) != _operators.end(); }
	bool	canJoin(const Client& client, std::string password) const;

	void	setPassword(const std::string& password) { _password = password; }
	void	setUserLimit(int limit) { _userLimit = limit; }
	void	addOperator(int clientId, Client* client);
	void	removeOperator(int clientId);
	void	addClient(int clientId, Client* client) { _Clients[clientId] = client; }
    std::string getTopic() const { return _topic; } ;
    std::string getName() const { return _name; } ;

    void setTopic(std::string topic) { _topic = topic; } ;

	//aux mode
	bool	isInvited(const std::string& name) const { return std::find(_invitedNames.begin(), _invitedNames.end(), name) != _invitedNames.end(); }
	void	addInvite(const std::string& name) { _invitedNames.push_back(name); }
	bool	hasPassword(const std::string& password) const { 
		if (password.empty()) {
			std::cout << "entra aqui? " << std::endl;
			return false;
		}
		std::cout << "hasPassword: " << (_password == password) << std::endl;
		for (unsigned int i = 0; i < _password.length(); i++) {
			std::cout << (int)_password[i] << std::endl;
		}
		return _password == password; };
	
private:
	Channel();
	std::string _name;
	std::string _mode;
	std::string	_password;
	std::string	_topic;
	bool		_inviteOnly;
	bool		_topicProtected;
	int			_userLimit;
	std::vector<std::string> _invitedNames; //FIXME - clientes que foram convidados p entrar em canal - MUDAR PARA FD
	std::map<int, Client*>   _Clients; //NOTE - sugestion: key ser uma bool - true se é operador
	std::map<int, Client*>	_operators; //NOTE - channel operators 
};

#endif // CHANNEL_HPP