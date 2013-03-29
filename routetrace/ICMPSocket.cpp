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
