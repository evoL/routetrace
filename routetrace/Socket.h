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
#include <cstdio>
#include <string>
#include <stdexcept>
#include "Packet.h"

class SocketException : public std::runtime_error {
public:
    SocketException(std::string what): std::runtime_error(what) {}
};

// Defines a raw IPv4 socket
class Socket {
public:
    Socket(): handle(0) {}
    virtual ~Socket();
    
    void setTTL(int ttl);
    virtual void send(const Packet& packet, std::string address);

protected:
    int handle;
    
    sockaddr_in parseAddress(std::string address);
};


#endif /* defined(__routetrace__Socket__) */
