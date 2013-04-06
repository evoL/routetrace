//
//  main.cpp
//  routetrace
//
//  Created by Rafał Hirsz on 29.03.2013.
//  Copyright (c) 2013 Rafał Hirsz. All rights reserved.
//

#include "ICMPSocket.h"
#include "ICMPPacket.h"
#include "IPPacket.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <unistd.h>

int main(int argc, const char * argv[]) {
    if (argc == 1) {
        std::cout << "Usage: routetrace (send|receive)" << std::endl;
        return 1;
    }
    
    std::string mode(argv[1]);
    
    if (mode == "send") {
        
        try {
            std::cout << "PID: " << getpid() << std::endl;
            
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
                std::cout << ", code " << packet.subtype() << std::endl;
                std::cout << "ID " << packet.id() << ", seq " << packet.sequence() << std::endl;
                
                std::cout << "Payload (" << packet.data().size() << " bytes):" << std::endl;
                
                std::cout << std::hex;
                
                int i = 0;
                for (ICMPPacket::Data::const_iterator it = packet.data().begin(); it != packet.data().end(); it++) {
                    if (*it < 16) std::cout << '0';
                    std::cout << (int)(*it) << ' ';
                    
                    if ((++i % 24) == 0) std::cout << std::endl;
                }
                
                std::cout << std::resetiosflags(std::ios::basefield);
                std::cout << std::endl;
                
                if (packet.type() == ICMP_TIME_EXCEEDED && packet.subtype() == ICMP_EXC_TTL) {
                    std::cout << "TTL is 0, let's see what's inside..." << std::endl;
                    
                    IPPacket ipPacket = IPPacket::fromData(packet.data());
                    
                    if (ipPacket.protocol() == IPPROTO_ICMP) {
                        std::cout << "A wild ICMP appeared!" << std::endl;
                        ICMPPacket *insidePacket = static_cast<ICMPPacket*>(ipPacket.payload());
                        
                        std::cout << "ICMP type: " << insidePacket->humanType();
                        std::cout << ", code " << insidePacket->subtype() << std::endl;
                        std::cout << "ID " << insidePacket->id() << ", seq " << insidePacket->sequence() << std::endl;
                    } else {
                        std::cout << "Nothing interesting :(" << std::endl;
                    }
                }
                
                else if (packet.type() == ICMP_ECHOREPLY) {
                    std::cout << "It's an echo reply!" << std::endl;
                }
                
                std::cout << std::endl;
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

