#ifndef MODE_HPP
# define MODE_HPP

# include "Includes.hpp"

class Mode: public ACommand {
public:
	Mode(Server& server, Client& client);
	~Mode() {};

	void parsing(std::istringstream &input);
	void execute();
	void print() const;

	std::string	getChannel() { return _channel; };
	std::string getMode() const { return _mode; };
	std::string getPassword() const { return _password; }
	int			getLimit() const { return _userLimit; }
	int			getClientId() const { return _clientId; }
private:
	std::string _channel;
	std::string _mode;
	std::string _password;
	int			_userLimit;
	int			_clientId;
	
	Mode();
	bool	isValidMode(char mode);
	void	extractKeyAndLimit(std::istringstream &input);
};

#endif // MODE_HPP