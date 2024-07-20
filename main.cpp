#include "Includes.hpp"

void check_port(std::string port) {
    for (unsigned int i = 0; i < port.length(); i++) {
        if (!isdigit(port[i]))
            throw IRCException("[WRONG PORT TYPE] The PORT must be only numbers");
    }
    if (std::atoi(port.c_str()) > 65535)
        throw IRCException("[WRONG PORT RANGE] The PORT must range between 0 and 65535");
}

void validate_input(int argc, char **argv) {
    if (argc != 3)
        throw IRCException("[WRONG ARGUMENTS] Try ./ircserv <PORT> <PASSWORD>");
    check_port(argv[1]);
}

int main(int argc, char **argv) {
    try {
        validate_input(argc, argv);
        Server server(std::atoi(argv[1]), argv[2]);
        server.run();
    } catch(const std::exception &e) {
        std::cout << RED << e.what() << RESET << std::endl;
    }
}