#include "Includes.hpp"

void   Client::getClientInfo() {
    struct protoent *proto = getprotobyname("tcp");
    std::cout << "Protocol:" << std::endl;
    std::cout << "\tp_name: " <<  proto->p_name << std::endl;
    std::cout << "\tp_aliases: " << proto->p_aliases << std::endl;
    std::cout << "\tp_proto: " << proto->p_proto << std::endl;

    char hostbuffer[256];
    int hostname;

    hostname = gethostname(hostbuffer, sizeof(hostbuffer));
    if (hostname == -1) {
        throw IRCException("[ERROR] gethostname went wrong");
    }

    std::cout << "Hostname: " << hostbuffer << std::endl;
}