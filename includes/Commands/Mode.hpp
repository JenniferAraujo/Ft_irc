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

inline std::string RPL_MODE(const std::string& nick, const std::string& user, const std::string& host, const std::string& channel, std::string mode, const std::string& userLimit, const std::string& password, const std::string& op) {
	std::string	msg = ":" + nick + "!" + user + "@" + host + " MODE " + channel;

	if (userLimit.empty() && password.empty() && op.empty()) {
		msg.append(" :");
 		if (mode[0] == '+' || mode[0] == '-')
			msg.append("");
		else
			msg.append("+"); 
		msg.append(mode).append("\r\n");
	}
	else {
		msg.append(" ").append(mode).append(" ");
		bool	flagUserLim = false;
		bool	flagOp = false;
		bool	flagPass = false;
		for (size_t i = 1; i < mode.length(); i++) {
			if (mode[i] == 'l' && !flagUserLim && !userLimit.empty()) {
				msg.append(userLimit).append(" ");
				flagUserLim = true;
			}
			else if (mode[i] == 'o' && !flagOp && !op.empty()) {
				msg.append(op).append(" ");
				flagOp = true;
			}
			else if (mode[i] == 'k' && !flagPass && !password.empty()) {
				msg.append(password).append(" ");
				flagPass = true;
			}
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
	std::string getExcMode() const { return _modeChar; };
	std::string	validParameter(Channel *channel);
	std::queue<std::string> getPassword() const { return _password; }
	std::queue<int>		getLimit() const { return _userLimit; }
	std::queue<std::string>	getClientNick() const { return _clientNick; }
	std::string			queueIntToString(std::queue<int> q);
	std::string			queueStrToString(std::queue<std::string> q);

private:
	std::string _channel;
	std::string _mode;
	std::string	_parameters;
	std::string	_modeChar;
	std::queue<int>			_userLimit;
	std::queue<int>			_msgUserLimit;
	std::queue<std::string> _msgPassword;
	std::queue<std::string> _password;
	std::queue<std::string>	_clientNick;
	std::queue<std::string>	_msgclientNick;
	
	Mode();
	bool	isValidMode(char mode);
	void	extractParameters();
};

#endif // MODE_HPP