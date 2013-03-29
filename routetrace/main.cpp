//
//  main.cpp
//  routetrace
//
//  Created by Rafał Hirsz on 29.03.2013.
//  Copyright (c) 2013 Rafał Hirsz. All rights reserved.
//

#include "ICMPSocket.h"
#include "ICMPPacket.h"
#include <iostream>
#include <errno.h>

int main(int argc, const char * argv[]) {
    try {
        ICMPSocket socket;
        ICMPPacket packet;
        
        packet.echo();
        
        socket.setTTL(1);
        socket.send(packet, "8.8.8.8");
    } catch (SocketException e) {
        std::cerr << e.what() << ": " << strerror(errno) << std::endl;
        exit(1);
    } catch (std::string e) {
        std::cerr << e << std::endl;
        exit(1);
    }
}

