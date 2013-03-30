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
#include <iomanip>
#include <string>

int main(int argc, const char * argv[]) {
    if (argc == 1) {
        std::cout << "Usage: routetrace (send|receive)" << std::endl;
        return 1;
    }
    
    std::string mode(argv[1]);
    
    if (mode == "send") {
        
        try {
            ICMPSocket socket;
            ICMPPacket packet;
            
            packet.echo();
            
            socket.setTTL(1);
            socket.send(packet, "8.8.8.8");
        } catch (SocketException e) {
            std::cerr << e.what() << ": " << e.cause() << std::endl;
            return 1;
        } catch (std::string e) {
            std::cerr << e << std::endl;
            return 1;
        }
        
    } else if (mode == "receive") {
        
        try {
            ICMPSocket socket;
            for (;;) {
                ICMPPacket packet = socket.receive();
                std::cout << std::resetiosflags(std::ios::basefield);
                
                std::cout << "Got an ICMP packet from " << socket.address() << std::endl;
                std::cout << "ICMP type: " << packet.humanType();
                std::cout << ", subtype: " << packet.humanSubtype() << std::endl;
                
                std::cout << "Payload (" << packet.data().size() << " bytes):" << std::endl;
                
                std::cout << std::hex;
                
                int i = 0;
                for (ICMPPacket::Data::const_iterator it = packet.data().begin(); it != packet.data().end(); it++) {
                    if (*it < 16) std::cout << '0';
                    std::cout << (int)(*it) << ' ';
                    
                    if ((++i % 24) == 0) std::cout << std::endl;
                }
                
                std::cout << std::endl << std::endl;
            }
        } catch (SocketException e) {
            std::cerr << e.what() << ": " << e.cause() << std::endl;
            return 1;
        } catch (std::string e) {
            std::cerr << e << std::endl;
            return 1;
        }
        
    } else {
        std::cout << "Usage: routetrace (send|receive)" << std::endl;
        return 1;
    }
}

