//
//  Socket.cpp
//  routetrace
//
//  Created by Rafał Hirsz on 29.03.2013.
//  Copyright (c) 2013 Rafał Hirsz. All rights reserved.
//

#include "Socket.h"

#include <cstdlib>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

Socket::~Socket() {
    close(handle);
}

void Socket::setTTL(int ttl) {
    if (setsockopt(handle, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0) {
        throw "Setting TTL failed";
    }
}

void Socket::send(const Packet& packet, std::string address) {
    sockaddr_in address_struct = parseAddress(address);
    
    if (sendto(handle, packet.structure(), packet.length(), 0, (sockaddr*)(&address_struct), sizeof(address_struct)) != packet.length()) {
        throw "Could not send the packet";
    }
}

sockaddr_in Socket::parseAddress(std::string address) {
    sockaddr_in result;
    memset(&result, 0, sizeof(result));
    
    result.sin_family = AF_INET;
    inet_pton(AF_INET, address.c_str(), &result.sin_addr);
    
    return result;
}