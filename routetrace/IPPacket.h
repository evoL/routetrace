//
//  IPPacket.h
//  routetrace
//
//  Created by Rafał Hirsz on 03.04.2013.
//  Copyright (c) 2013 Rafał Hirsz. All rights reserved.
//

#ifndef __routetrace__IPPacket__
#define __routetrace__IPPacket__

#include "Packet.h"

#include <netinet/ip.h>

class IPPacket : public Packet {
    ip packet;
    Packet* payloadPacket;
public:
    IPPacket();
    IPPacket(ip packet): packet(packet), Packet() {}
    IPPacket(ip packet, Data data);
    virtual ~IPPacket();
    
    inline char protocol() { return packet.ip_p; }
    
    Packet* payload();
    
    virtual inline int length() const { return packet.ip_hl * 4; }
    virtual inline const void* structure() const { return &packet; }
    
    static IPPacket fromData(const Data& data);
    
private:
    Packet* createPayload();
};

#endif /* defined(__routetrace__IPPacket__) */
