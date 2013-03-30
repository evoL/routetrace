//
//  ICMPSocket.h
//  routetrace
//
//  Created by Rafał Hirsz on 29.03.2013.
//  Copyright (c) 2013 Rafał Hirsz. All rights reserved.
//

#ifndef __routetrace__ICMPSocket__
#define __routetrace__ICMPSocket__

#include "Socket.h"
#include "ICMPPacket.h"

class ICMPSocket : public Socket {
public:
    ICMPSocket();
    
    virtual void send(const Packet& packet, std::string address);
    virtual void send(const ICMPPacket& packet, std::string address);
    
    ICMPPacket receive();
};

#endif /* defined(__routetrace__ICMPSocket__) */
