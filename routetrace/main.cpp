//
//  main.cpp
//  routetrace
//
//  Created by Rafał Hirsz on 29.03.2013.
//  Copyright (c) 2013 Rafał Hirsz. All rights reserved.
//

#include "ICMPSocket.h"
#include "ICMPPacket.h"

int main(int argc, const char * argv[]) {
    ICMPSocket socket;
    ICMPPacket packet;
    
    packet.echo();
    
    socket.setTTL(1);
    socket.send(packet, "8.8.8.8");
}

