// Rafał Hirsz, nr 247955

#include "IPPacket.h"
#include "ICMPPacket.h"

IPPacket::IPPacket(ip packet, Data data): Packet(data), packet(packet) {
    payloadPacket = createPayload();
}

IPPacket::~IPPacket() {
    delete payloadPacket;
}

Packet* IPPacket::payload() {
    if (payloadPacket == NULL) throw "Cannot get payload for an unknown protocol";

    return payloadPacket;
}

Packet* IPPacket::createPayload() {
    unsigned char *dataPointer = &rawData[0];
    
    switch (packet.ip_p) {
        case IPPROTO_ICMP:
            return new ICMPPacket(*(icmp*)(dataPointer), ICMPPacket::Data(dataPointer + 8, (unsigned char *)&rawData[rawData.size()]));
        default: return NULL;
    }
}

IPPacket IPPacket::fromData(const Packet::Data& data) {
    ip* packet = (ip *)&data[0];
    unsigned char* dataStart = (unsigned char*)&data[packet->ip_hl * 4];
    
    return IPPacket(*packet, Data(dataStart, (unsigned char*)&data[data.size()]));
}