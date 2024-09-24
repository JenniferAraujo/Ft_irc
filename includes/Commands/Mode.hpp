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

inline std::string format_mode(const std::string& mode, const std::string& userLimit, const std::string& password, const std::string& op, Channel& channel){
	std::string plus = "+";
	std::string minus = "-";
	std::cout << "ENTRA AQUI\n";
	for(int i = 0; i < (int)mode.length(); ++i){
		if(mode[i] == '+' || mode[i] == 'i' || mode[i] == 'o' || mode[i] == 't' || mode[i] == 'l' || mode[i] == 'k'){
			if(mode[i] == '+')
				i++;
			while(mode[i] != '+' && mode[i] != '-' && mode[i] != '\0'){
				std::cout << "AAAAAA: " << mode[i] << std::endl;
				if (mode[i] == 'l' && atoi(userLimit.c_str()) != channel.getUserLimit())
					plus += mode[i];
				else if (mode[i] == 'o' && channel.getOperatorByNick(op) == NULL)
					plus += mode[i];
				else if (mode[i] == 'k' && password != channel.getPassword())
					plus += mode[i];
				else if (mode[i] == 'i' && !channel.getInviteOnly()){
					std::cout << "IF: " << mode[i] << std::endl;
					plus += mode[i];
				}
				else if (mode[i] == 't' && !channel.getTopicProtected())
					plus += mode[i];
				std::cout << "Plus: " << plus << std::endl;
				i++;
			}
		}
		else if (mode[i] == '-'){
			i++;
			while(mode[i] != '+' && mode[i] != '-' && mode[i] != '\0'){
				if (mode[i] == 'l' && atoi(userLimit.c_str()) != channel.getUserLimit())
					minus += mode[i];
				else if (mode[i] == 'o')
					minus += mode[i];
				else if (mode[i] == 'k' && !channel.getPassword().empty())
					minus += mode[i];
				else if (mode[i] == 'i' && channel.getInviteOnly())
					minus += mode[i];
				else if (mode[i] == 't' && channel.getTopicProtected())
					minus += mode[i];
				std::cout << "Minus: " << minus << std::endl;
				i++;
			}
		}
	}
	std::string empty;
	if(plus.length() == 1 && minus.length() == 1)
		return (empty);
	return plus.append(minus);
}

inline std::string RPL_MODE(const std::string& nick, const std::string& user, const std::string& host, const std::string& channelName, const std::string& mode, const std::string& userLimit, const std::string& password, const std::string& op, Channel& channel) {
	std::string	msg = ":" + nick + "!" + user + "@" + host + " MODE " + channelName;

	if (userLimit == "-1" && password.empty() && op.empty()) {
		std::string formatedMode = format_mode(mode, userLimit, password, op, channel);
		if(formatedMode.empty())
			return(formatedMode);
		msg.append(" :");
/* 		if (mode[0] == '+' || mode[0] == '-')
			msg.append("");
		else
			msg.append("+"); */
		msg.append(mode).append("\r\n");
	}
	else {
 		//if (mode[0] == '+' || mode[0] == '-')
			msg.append(" ");
		/*else
			msg.append(" +"); */
		std::string formatedMode = format_mode(mode, userLimit, password, op, channel);
		if(formatedMode.empty())
			return(formatedMode);
		msg.append(mode).append(" ");
		for (size_t i = 1; i < mode.length(); i++) {
			if (i + 1 >= mode.length())
				msg.append(":");
			if (mode[i] == 'l' && atoi(userLimit.c_str()) != channel.getUserLimit())
				msg.append(userLimit).append(" ");
			else if (mode[i] == 'o' && channel.getOperatorByNick(op) == NULL)
				msg.append(op).append(" ");
			else if (mode[i] == 'k' && password != channel.getPassword())
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