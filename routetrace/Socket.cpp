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

Socket::Socket() {}

Socket::~Socket() {
    close(fd);
}

void Socket::setTTL(int ttl) {
    if (setsockopt(fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) < 0) {
        throw SocketException("Setting TTL failed");
    }
}

void Socket::send(const Packet& packet, std::string address) {
    sockaddr_in address_struct = addressFromString(address);
    lastAddress = address;
    
    // Make a copy of the raw header
    unsigned char* dataPointer = (unsigned char*)(packet.structure());
    Packet::Data data(dataPointer, dataPointer + packet.length());
    
    // Append the data to the header
    data.insert(data.end(), packet.data().begin(), packet.data().end());
    
    if (sendto(fd, &data[0], data.size(), 0, (sockaddr*)(&address_struct), sizeof(address_struct)) != data.size()) {
        throw SocketException("Could not send the packet");
    }
}

void Socket::bind(int port) {
    sockaddr_in address;
    
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;
    
    if (::bind(fd, (sockaddr*)&address, sizeof(address)) < 0) {
        throw SocketException("Could not bind socket");
    }
}

unsigned char* Socket::receiveData(ssize_t *length) {
    sockaddr_in sender;
    socklen_t senderLength = sizeof(sender);
    unsigned char *buffer = receiveBuffer;
    
    ssize_t remaining = recvfrom(fd, buffer, IP_MAXPACKET, 0, (sockaddr*)(&sender), &senderLength);
    
    if (remaining < 0)
        throw SocketException("Could not receive from the socket");
    
    lastAddress = addressToString(sender);
    
    // Bypass the IP packet
    ip* ipPacket = (ip*)buffer;
    remaining -= ipPacket->ip_hl * 4;
    buffer += ipPacket->ip_hl * 4;
    
    *length = remaining;
    return buffer;
}

sockaddr_in Socket::addressFromString(std::string address) {
    sockaddr_in result;
    memset(&result, 0, sizeof(result));
    
    result.sin_family = AF_INET;
    inet_pton(AF_INET, address.c_str(), &result.sin_addr);
    
    return result;
}

std::string Socket::addressToString(sockaddr_in& address) {
    char str[20];
    inet_ntop(AF_INET, &(address.sin_addr), str, sizeof(str));
    
    return std::string(str);
}