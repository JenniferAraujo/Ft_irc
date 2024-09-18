#ifndef MODE_HPP
# define MODE_HPP

# include "Includes.hpp"

inline std::string RPL_ONLYMODE(const std::string& nick, const std::string& source, const std::string& channel, const std::string& mode, const std::string& userLimit, const std::string& password) {
	std::string	msg = ":" + source + " 324 " + nick + " " + channel;

	if (userLimit == "-1" && password.empty())
		msg.append(" :+").append(mode).append("\r\n");
	else {
		msg.append(" +").append(mode).append(" ");
		for (size_t i = 0; i < mode.length(); i++) {
			if (i + 1 >= mode.length())
				msg.append(":");
			if (mode[i] == 'l')
				msg.append(userLimit).append(" ");
			else if (mode[i] == 'k')
				msg.append(password).append(" ");
		}
		msg.append("\r\n");
	}
	return msg;
}

inline std::string RPL_MODE(const std::string& nick, const std::string& user, const std::string& host, const std::string& channel, const std::string& mode, const std::string& userLimit, const std::string& password, const std::string& op) {
	std::string	msg = ":" + nick + "!" + user + "@" + host + " MODE " + channel;

	if (userLimit == "-1" && password.empty() && op.empty()) {
		msg.append(" :");
		if (mode[0] == '+' || mode[0] == '-')
			msg.append("");
		else
			msg.append("+");
		msg.append(mode).append("\r\n");
	}
	else {
		if (mode[0] == '+' || mode[0] == '-')
			msg.append(" ");
		else
			msg.append(" +");
		msg.append(mode).append(" ");
		for (size_t i = 1; i < mode.length(); i++) {
			if (i + 1 >= mode.length())
				msg.append(":");
			if (mode[i] == 'l')
				msg.append(userLimit).append(" ");
			else if (mode[i] == 'o')
				msg.append(op).append(" ");
			else if (mode[i] == 'k')
				msg.append(password).append(" ");
		}
		msg.append("\r\n");
	}
	return msg;
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