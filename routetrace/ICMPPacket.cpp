//
//  ICMPPacket.cpp
//  routetrace
//
//  Created by Rafał Hirsz on 29.03.2013.
//  Copyright (c) 2013 Rafał Hirsz. All rights reserved.
//

#include "ICMPPacket.h"

void ICMPPacket::type(ICMPPacketType type) {
    switch (type) {
    case ICMP_PACKET_ECHO:
        packet.icmp_type = ICMP_ECHO;
        break;
    default:
        throw "Invalid ICMP packet type";
    }
}