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

class ICMPPacket : public Packet {
    icmp packet;
    
    static int id;
    static int sequence;
public:
    enum Type {
        ICMP_PACKET_ECHO
    };
    enum SubType {
        ICMP_PACKET_NORMAL
    };
    
    ICMPPacket();
    
    void echo();
    void type(Type type, SubType subtype = ICMP_PACKET_NORMAL);
    
    virtual inline int length() const { return 8; } // ICMP header length
    virtual inline const void* structure() const { return &packet; }
    
private:
    u_short computeChecksum();
};

#endif /* defined(__routetrace__ICMPPacket__) */
