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
    
    static int ID;
    static int Sequence;
public:
    enum Type {
        ICMP_TYPE_ECHO,
        ICMP_TYPE_ECHO_REPLY,
        ICMP_TYPE_TIME_EXCEEDED,
        ICMP_TYPE_UNKNOWN
    };
    enum SubType {
        ICMP_SUBTYPE_NORMAL,
        ICMP_SUBTYPE_TIME_EXCEEDED_TTL,
        ICMP_SUBTYPE_UNKNOWN
    };
    
    ICMPPacket();
    ICMPPacket(icmp packet): packet(packet), Packet() {}
    ICMPPacket(icmp packet, Data data): packet(packet), Packet(data) {}
    
    void echo();
    
    inline int id() { return packet.icmp_id; }
    inline int sequence() { return packet.icmp_seq; }
    
    SubType subtype();
    
    Type type();
    void type(Type type, SubType subtype = ICMP_SUBTYPE_NORMAL);
    
    virtual inline int length() const { return 8; } // ICMP header length
    virtual inline const void* structure() const { return &packet; }
    
private:
    u_short computeChecksum();
};

#endif /* defined(__routetrace__ICMPPacket__) */
