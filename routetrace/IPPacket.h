// Rafał Hirsz, nr 247955

#ifndef __routetrace__IPPacket__
#define __routetrace__IPPacket__

#include "Packet.h"

#include <netinet/ip.h>

class IPPacket : public Packet {
    ip packet;
    Packet* payloadPacket;
public:
    IPPacket();
    IPPacket(ip packet): Packet(), packet(packet) {}
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
