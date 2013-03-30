//
//  Socket.h
//  routetrace
//
//  Created by Rafał Hirsz on 29.03.2013.
//  Copyright (c) 2013 Rafał Hirsz. All rights reserved.
//

#ifndef __routetrace__Socket__
#define __routetrace__Socket__

#include <netinet/in.h>
#include <string>
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

protected:
    int handle;
    
    sockaddr_in parseAddress(std::string address);
};


#endif /* defined(__routetrace__Socket__) */
