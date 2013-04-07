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
    if ((fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0) {
        throw SocketException("Could not create socket");
    }
}

void ICMPSocket::send(const Packet&, std::string) {
    throw "Attempted to send a non-ICMP packet through a ICMP socket";
}

void ICMPSocket::send(const ICMPPacket& packet, std::string address) {
    Socket::send(static_cast<const Packet&>(packet), address);
}

ICMPPacket ICMPSocket::receive() {
    ssize_t length;
    unsigned char* buffer = receiveData(&length);
    unsigned char* dataPointer = buffer + 8;
    
    return ICMPPacket(*(icmp*)buffer, ICMPPacket::Data(dataPointer, dataPointer + (length - 8)));
}