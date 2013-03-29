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
#include "Packet.h"

// Defines a raw IPv4 socket
class Socket {
public:
    Socket(): handle(0), valid(false) {}
    virtual ~Socket();
    
    void setTTL(int ttl);
    void send(const Packet& packet, std::string address);

protected:
    int handle;
    bool valid;
    
    sockaddr_in parseAddress(std::string address);
};


#endif /* defined(__routetrace__Socket__) */
