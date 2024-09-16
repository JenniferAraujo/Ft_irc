#ifndef MODE_HPP
# define MODE_HPP

# include "Includes.hpp"

inline std::string RPL_MODE(const std::string& source, const std::string& channel, const std::string& target, const std::string& mode) {
    return ":" + source + " 324 " + target + " " + channel + " " + mode + "\r\n";
}

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
	std::string		getClientNick() const { return _clientNick; }
private:
	std::string _channel;
	std::string _mode;
	std::string _password;
	std::string	_parameters;
	int			_userLimit;
	std::string	_clientNick;
	
	Mode();
	bool	isValidMode(char mode);
	void	extractParameters();
};

#endif // MODE_HPP