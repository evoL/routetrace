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
#include <string>

#ifdef __APPLE__
#define ICMP_TIME_EXCEEDED ICMP_TIMXCEED
#define ICMP_EXC_TTL       ICMP_TIMXCEED_INTRANS
#endif

class ICMPPacket : public Packet {
    icmp packet;
    
    static int ID;
    static int Sequence;
public:
    typedef unsigned char Type;
    typedef int SubType;
    
    ICMPPacket();
    ICMPPacket(int id, int seq);
    ICMPPacket(icmp packet): packet(packet), Packet() {}
    ICMPPacket(icmp packet, Data data): packet(packet), Packet(data) {}
    
    void echo();
    
    inline int id() { return packet.icmp_id; }
    inline int sequence() { return packet.icmp_seq; }
    
    inline SubType subtype() { return (SubType)(packet.icmp_code); }
    
    inline Type type() { return packet.icmp_type;}
    void type(Type type, SubType subtype = 0);
    
    std::string humanType();
    
    virtual inline int length() const { return 8; } // ICMP header length
    virtual inline const void* structure() const { return &packet; }
    
    static ICMPPacket fromData(const Data& data);
private:
    u_short computeChecksum();
};

#endif /* defined(__routetrace__ICMPPacket__) */
