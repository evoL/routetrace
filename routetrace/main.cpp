//
//  main.cpp
//  routetrace
//
//  Created by Rafał Hirsz on 29.03.2013.
//  Copyright (c) 2013 Rafał Hirsz. All rights reserved.
//

#include "ICMPSocket.h"
#include "ICMPPacket.h"
#include "Selector.h"
#include <iostream>

using namespace std;

int main(int argc, const char * argv[]) {
    if (argc == 1) {
        cout << "Usage: routetrace (ip)" << endl;
        return 1;
    }
    
    try {
        ICMPSocket readSocket;
        Selector selector;
        selector.timeout(2);
        
        for (;;) {
            selector.read(readSocket);
            
            if (selector.run()) {
                cout << "Received packet!" << endl;
                
                ICMPPacket packet = readSocket.receive();
                cout << "Source address: " << readSocket.address() << endl;
                cout << "Type: " << packet.humanType() << endl;
                
                break;
            }
            
            cout << "Waiting another 2 sec..." << endl;
            selector.clear();
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

