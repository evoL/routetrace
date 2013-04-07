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
#include "Selector.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unistd.h>
#include <sys/time.h>

using namespace std;

int main(int argc, const char * argv[]) {
    if (argc == 1) {
        cout << "Usage: routetrace (ip)" << endl;
        return 1;
    }
    
    string target_ip = argv[1];
    
    cout.precision(4);
    
    try {
        ICMPSocket socket;
        Selector selector;
        selector.timeout(1);
        
        vector<timeval> starts;
        set<string> addresses;
        int times[90];
        float sum;
        bool isTimeout;
        
        int icmpid = getpid() & 0xffff;
        int sequence = 0;
        bool running = true;
        timeval start;
        
        for (int ttl = 1; running && (ttl <= 30); ttl++) {
            cout << ttl;
            cout.flush();
            
            // Send 3 packets
            for (int i = 0; i < 3; i++) {
                ICMPPacket ping(icmpid, ++sequence);
                ping.echo();
                
//                cout << "  >> " << icmpid << " (" << sequence << ")" << endl;
                
                gettimeofday(&start, NULL);
                starts.push_back(start);
                
                socket.setTTL(ttl);
                socket.send(ping, target_ip);
            }
            
            sum = 0;
            isTimeout = false;
            addresses.clear();
            
            // Try to read 3 packets
            for (int i = 0; i < 3; i++) {                
                selector.read(socket);
                if (selector.run()) {
                    ICMPPacket packet = socket.receive();
                    
                    timeval end;
                    gettimeofday(&end, NULL);
                    
                    int time;
                    
//                    cout << "  << ";
                    
                    if (packet.type() == ICMP_TIME_EXCEEDED && packet.subtype() == ICMP_EXC_TTL) {
                        IPPacket ip_packet = IPPacket::fromData(packet.data());
                        ICMPPacket *inside_packet = static_cast<ICMPPacket*>(ip_packet.payload());
                        
                        // Check ID and sequence
                        if ((inside_packet->id() == icmpid) && (inside_packet->sequence() > (sequence - 3)) && (inside_packet->sequence() <= sequence)) {
                            // Calculate elapsed time
                            start = starts[inside_packet->sequence() - 1];
                            time = (int)((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec));
                            times[inside_packet->sequence() - 1] = time;
                            
                            sum += (float)time / 1000;
                            
                            // Get the address
                            addresses.insert(socket.address());
                        } else {
                            // We get some weird packets.
                            i--;
                        }
                        
//                        cout << inside_packet->id() << " (" << inside_packet->sequence() << ") | " << packet.humanType();
//                        cout << " | " << socket.address();
//                        cout << " | " << fixed << ((float)time / 1000) << "ms";
                    } else if (packet.type() == ICMP_ECHOREPLY) {

                        if ((packet.id() == icmpid) && (packet.sequence() > (sequence - 3)) && (packet.sequence() <= sequence)) {
                            // Calculate elapsed time
                            start = starts[packet.sequence() - 1];
                            time = (int)((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec));
                            times[packet.sequence() - 1] = time;
                            
                            sum += (float)time / 1000;
                            
                            // Get the address
                            addresses.insert(socket.address());
                        } else {
                            i--;
                        }

//                        cout << packet.id() << " (" << packet.sequence() << ") | " << packet.humanType();
//                        cout << " | " << socket.address();
//                        cout << " | " << fixed << ((float)time / 1000) << "ms";

                        running = false;
                    } else {
//                        cout << "??? (trying again)";
                        i--;
                    }
                    
//                    cout << endl;
                    
                } else {
                    // Timeout :/
                    times[sequence - 2 + i] = -1;
                    
//                    cout << "  *  Timeout" << endl;
                    isTimeout = true;
                }
                selector.clear();
            }
            
            // Post-processing
            for (set<string>::iterator it = addresses.begin(); it != addresses.end(); it++) {
                cout << " " << *it;
            }
            cout << " ";
            
            if (isTimeout) {
                cout << "???";
            } else {
                cout << (sum / 3) << "ms";
            }
            cout << endl;
        }
        
    } catch (SocketException e) {
        cerr << "Error: " << e.what() << " - " << e.cause() << endl;
        return 1;
    } catch (std::string e) {
        cerr << "Error: " << e << endl;
        return 1;
    }
    
    return 0;
}

