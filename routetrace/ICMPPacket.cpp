//
//  ICMPPacket.cpp
//  routetrace
//
//  Created by Rafał Hirsz on 29.03.2013.
//  Copyright (c) 2013 Rafał Hirsz. All rights reserved.
//

#include "ICMPPacket.h"

#include <unistd.h>

#ifdef __APPLE__
#define ICMP_TIME_EXCEEDED ICMP_TIMXCEED
#define ICMP_EXC_TTL       ICMP_TIMXCEED_INTRANS
#endif

int ICMPPacket::ID = getpid();
int ICMPPacket::Sequence = 0;

ICMPPacket::ICMPPacket() {
    packet.icmp_id = ID;
    packet.icmp_seq = ++Sequence;
}

void ICMPPacket::echo() {
    type(ICMP_TYPE_ECHO);
}

ICMPPacket::SubType ICMPPacket::subtype() {
    switch (packet.icmp_type) {
        case ICMP_TIME_EXCEEDED:
            switch (packet.icmp_code) {
                case ICMP_EXC_TTL: return ICMP_SUBTYPE_TIME_EXCEEDED_TTL;
                default: return ICMP_SUBTYPE_UNKNOWN;
            }            
        default: return ICMP_SUBTYPE_NORMAL;
    }
}

ICMPPacket::Type ICMPPacket::type() {
    switch (packet.icmp_type) {
        case ICMP_ECHO: return ICMP_TYPE_ECHO;
        case ICMP_ECHOREPLY: return ICMP_TYPE_ECHO_REPLY;
        case ICMP_TIME_EXCEEDED: return ICMP_TYPE_TIME_EXCEEDED;
        default: return ICMP_TYPE_UNKNOWN;
    }
}

void ICMPPacket::type(ICMPPacket::Type type, ICMPPacket::SubType subtype) {
    switch (type) {
        case ICMP_TYPE_ECHO:
            packet.icmp_type = ICMP_ECHO;
            break;
        case ICMP_TYPE_ECHO_REPLY:
            packet.icmp_type = ICMP_ECHOREPLY;
            break;
        case ICMP_TYPE_TIME_EXCEEDED:
            packet.icmp_type = ICMP_TIME_EXCEEDED;
            break;
        default:
            throw "Invalid ICMP type";
    }
    
    switch (subtype) {
        case ICMP_SUBTYPE_NORMAL:
            packet.icmp_code = 0;
            break;
        case ICMP_SUBTYPE_TIME_EXCEEDED_TTL:
            packet.icmp_code = ICMP_EXC_TTL;
            break;
        default:
            throw "Invalid ICMP subtype/code";
    }
    
    packet.icmp_cksum = 0;
    packet.icmp_cksum = computeChecksum();
}

std::string ICMPPacket::humanType() {
    switch (type()) {
        case ICMP_TYPE_ECHO:          return "Echo";
        case ICMP_TYPE_ECHO_REPLY:    return "Echo reply";
        case ICMP_TYPE_TIME_EXCEEDED: return "Time exceeded";
        default: return "Unknown";
    }
}

std::string ICMPPacket::humanSubtype() {
    switch (subtype()) {
        case ICMP_SUBTYPE_NORMAL: return "Normal";
        case ICMP_SUBTYPE_TIME_EXCEEDED_TTL: return "TTL at 0";
        default: return "Unknown";
    }
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

