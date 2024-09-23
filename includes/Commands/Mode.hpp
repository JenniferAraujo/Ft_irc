#ifndef MODE_HPP
# define MODE_HPP

# include "Includes.hpp"

inline std::string RPL_MODE(const std::string& nick, const std::string& user, const std::string& host, const std::string& channel, const std::string& mode, const std::string& userLimit, const std::string& password, const std::string& op) {
	if (mode == "i" || mode == "+i" || mode == "-i")
		return ":" + nick + "!" + user + "@" + host + " MODE " + channel + " :" + (mode == "i" ? "+" : "") + mode + "\r\n";
	else if (mode == "t" || mode == "+t" || mode == "-t")
		return ":" + nick + "!" + user + "@" + host + " MODE " + channel + " :" + (mode == "t" ? "+" : "") + mode + "\r\n";
	else if (mode == "l" || mode == "+l")
		return ":" + nick + "!" + user + "@" + host + " MODE " + channel + (mode == "l" ? " +" : " ") + mode + " :" + userLimit + "\r\n";
	else if ( mode == "-l")
		return ":" + nick + "!" + user + "@" + host + " MODE " + channel + " :" + mode + "\r\n";
	else if (mode == "k" || mode == "+k" || mode == "-k")
		return ":" + nick + "!" + user + "@" + host + " MODE " + channel + (mode == "k" ? " +" : " ") + mode + (mode == "+k" || mode == "k" ? " :" : " ") + password + "\r\n";
	else if (mode == "o" || mode == "+o" || mode == "-o")
		return ":" + nick + "!" + user + "@" + host + " MODE " + channel + (mode == "o" ? " +" : " ") + mode + " :" + op + "\r\n";
	return ":" + nick + "!" + user + "@" + host + " MODE " + channel + " :" + (mode == "i" ? "+" : "") + mode + "\r\n";
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
	std::string	_modeChar;
	int			_userLimit;
	std::string	_clientNick;
	
	Mode();
	bool	isValidMode(char mode);
	void	extractParameters();
};

#endif // MODE_HPP