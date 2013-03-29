//
//  ICMPPacket.h
//  routetrace
//
//  Created by Rafał Hirsz on 29.03.2013.
//  Copyright (c) 2013 Rafał Hirsz. All rights reserved.
//

#ifndef __routetrace__ICMPPacket__
#define __routetrace__ICMPPacket__

#include "Packet.h"

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

enum ICMPPacketType {
    ICMP_PACKET_ECHO
};

class ICMPPacket : public Packet {
    icmp packet;
public:
    void type(ICMPPacketType type);
    
    virtual inline int length() const { return 8; } // ICMP header length
    virtual inline const void* structure() const { return &packet; }
};

#endif /* defined(__routetrace__ICMPPacket__) */
