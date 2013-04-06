//
//  ICMPPacket.cpp
//  routetrace
//
//  Created by Rafał Hirsz on 29.03.2013.
//  Copyright (c) 2013 Rafał Hirsz. All rights reserved.
//

#include "ICMPPacket.h"

#include <unistd.h>
#include <sstream>

int ICMPPacket::ID = getpid();
int ICMPPacket::Sequence = 0;

ICMPPacket::ICMPPacket() {
    packet.icmp_id = ID;
    packet.icmp_seq = ++Sequence;
}

void ICMPPacket::echo() {
    type(ICMP_ECHO);
}

void ICMPPacket::type(ICMPPacket::Type type, ICMPPacket::SubType subtype) {
    packet.icmp_type = type;
    packet.icmp_code = subtype;
    
    packet.icmp_cksum = 0;
    packet.icmp_cksum = computeChecksum();
}

std::string ICMPPacket::humanType() {
    switch (packet.icmp_type) {
        case ICMP_ECHO:          return "Echo (8)";
        case ICMP_ECHOREPLY:    return "Echo reply (0)";
        case ICMP_TIME_EXCEEDED: return "Time exceeded (11)";
        default:
            std::stringstream str;
            str << "Unknown (" << (int)(packet.icmp_type) << ")";
            return str.str();
    }
}

ICMPPacket ICMPPacket::fromData(const Data& data) {
    icmp* packet = (icmp*)&data[0];
    unsigned char* dataPointer = (unsigned char*)&data[8];
    
    return ICMPPacket(*packet, Data(dataPointer, (unsigned char*)&data[data.size()]));
}

// Based on in_cksum from ping.c of iputils-20071127
u_short ICMPPacket::computeChecksum() {
    register int nleft = 8;
    const u_short *w = reinterpret_cast<u_short*>(&packet);
    register u_short answer;
    register int sum = 0;
    
    /*
     *  Our algorithm is simple, using a 32 bit accumulator (sum),
     *  we add sequential 16 bit words to it, and at the end, fold
     *  back all the carry bits from the top 16 bits into the lower
     *  16 bits.
     */
    while (nleft > 1)  {
        sum += *w++;
        nleft -= 2;
    }
    
    /* mop up an odd byte, if necessary */
    if (nleft == 1)
        sum += htons(*(u_char *)w << 8);
    
    /*
     * add back carry outs from top 16 bits to low 16 bits
     */
    sum = (sum >> 16) + (sum & 0xffff); /* add hi 16 to low 16 */
    sum += (sum >> 16);         /* add carry */
    answer = ~sum;              /* truncate to 16 bits */

    return answer;
}

