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
#include <iomanip>
#include <string>
#include <vector>
#include <set>
#include <unistd.h>
#include <sys/time.h>

using namespace std;

const int MAXHOPS = 30;

int main(int argc, const char * argv[]) {
    if (argc == 1) {
        cout << "Usage: routetrace (ip)" << endl;
        return 1;
    }
    
    string target_ip = argv[1];
    
    try {
        ICMPSocket socket;
        Selector selector;
        selector.timeout(1);
        
        vector<timeval> starts;
        set<string> addresses;
        timeval start;
        float sum;
        bool isTimeout;
        
        int icmpid = getpid() & 0xffff;
        int sequence = 0;
        bool running = true;
        
        for (int ttl = 1; running && (ttl <= MAXHOPS); ttl++) {
            cout << setw(2) << right << ttl << "  ";
            cout.flush();
            
            // Send 3 packets
            for (int i = 0; i < 3; i++) {
                ICMPPacket ping(icmpid, ++sequence);
                ping.echo();
                
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
                    int received_id, received_seq;
                    bool received = false;
                    
                    if (packet.type() == ICMP_TIME_EXCEEDED && packet.subtype() == ICMP_EXC_TTL) {
                        IPPacket ip_packet = IPPacket::fromData(packet.data());
                        ICMPPacket *inside_packet = static_cast<ICMPPacket*>(ip_packet.payload());
                        
                        received = true;
                        received_id = inside_packet->id();
                        received_seq = inside_packet->sequence();
                        
                    } else if (packet.type() == ICMP_ECHOREPLY) {
                        received = true;
                        received_id = packet.id();
                        received_seq = packet.sequence();
                    }
                    
                    if (received) {
                        if ((received_id == icmpid) && (received_seq > (sequence - 3)) && (received_seq <= sequence)) {
                            // Calculate elapsed time
                            start = starts[received_seq - 1];
                            time = (int)((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec));
                            
                            sum += (float)time / 1000;
                            
                            // Get the address
                            addresses.insert(socket.address());
                            
                            // Stop on valid echo reply
                            if (packet.type() == ICMP_ECHOREPLY) {
                                running = false;
                            }
                        } else {
                            // This packet is weird, try receiving another one
                            i--;
                        }
                    } else {
                        i--;
                    }
                } else {
                    isTimeout = true;
                }
                selector.clear();
            }
            
            // Write the output
            set<string>::iterator it = addresses.begin();
            if (it != addresses.end()) {
                cout << setw(15) << left << *it;
                it++;
            }
            for (; it != addresses.end(); it++) {
                cout << endl << "    ";
                cout << setw(15) << left << *it;
            }
            
            if (sum == 0) {
                cout << "*";
            } else if (isTimeout) {
                cout << " - ???";
            } else {
                cout << " - " << setprecision(4) << (sum / 3) << " ms";
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

