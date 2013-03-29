//
//  ICMPSocket.cpp
//  routetrace
//
//  Created by Rafał Hirsz on 29.03.2013.
//  Copyright (c) 2013 Rafał Hirsz. All rights reserved.
//

#include "ICMPSocket.h"

#include <sys/socket.h>

ICMPSocket::ICMPSocket() {
    handle = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    valid = (handle >= 0);
}

void ICMPSocket::send(const Packet& packet, std::string address) {
    throw "Attempted to send a non-ICMP packet through a ICMP socket";
}

void ICMPSocket::send(const ICMPPacket& packet, std::string address) {
    Socket::send(static_cast<const Packet&>(packet), address);
}