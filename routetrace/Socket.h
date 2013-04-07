// Rafał Hirsz, nr 247955

#ifndef __routetrace__Socket__
#define __routetrace__Socket__

#include <netinet/in.h>
#include <netinet/ip.h>
#include <string>
#include <cstring>
#include <stdexcept>
#include <errno.h>
#include "Packet.h"

class SocketException : public std::runtime_error {
    int error_code;
public:
    SocketException(std::string what): std::runtime_error(what), error_code(errno) {}
    
    inline int code() const { return error_code; }
    std::string cause() const {
        return std::string(strerror(error_code));
    }
};

// Defines a raw IPv4 socket
class Socket {
public:
    Socket();
    virtual ~Socket();
    
    void setTTL(int ttl);
    
    virtual void send(const Packet& packet, std::string address);
    void bind(int port);
    
    inline std::string address() const { return lastAddress; }
    inline int handle() const { return fd; }

protected:
    int fd;
    std::string lastAddress;
    unsigned char receiveBuffer[IP_MAXPACKET+1];
    
    unsigned char* receiveData(ssize_t *length);
    
    sockaddr_in addressFromString(std::string address);
    std::string addressToString(sockaddr_in& address);
};


#endif /* defined(__routetrace__Socket__) */
