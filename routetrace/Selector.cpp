//
//  Selector.cpp
//  routetrace
//
//  Created by Rafał Hirsz on 06.04.2013.
//  Copyright (c) 2013 Rafał Hirsz. All rights reserved.
//

#include "Selector.h"

#include <algorithm>

Selector::Selector() {
    FD_ZERO(&readSet);
    FD_ZERO(&writeSet);
    
    timeoutValue.tv_sec = 0;
    timeoutValue.tv_usec = 0;
    waitInfinitely = false;
    maxFd = -1;
}

Selector::~Selector() {}

void Selector::read(const Socket& s) {
    FD_SET(s.handle(), &readSet);
    if (maxFd < s.handle()) maxFd = s.handle();
    
    reads.insert(&s);
}

void Selector::write(const Socket& s) {
    FD_SET(s.handle(), &writeSet);
    if (maxFd < s.handle()) maxFd = s.handle();
    
    writes.insert(&s);
}

void Selector::clear() {
    FD_ZERO(&readSet);
    FD_ZERO(&writeSet);
    
    reads.clear();
    writes.clear();
    maxFd = -1;
}

void Selector::timeout(time_t sec) {
    waitInfinitely = false;
    timeoutValue.tv_sec = sec;
}

bool Selector::run() {
    int result;
    
    if ((result = select(maxFd + 1, &readSet, &writeSet, NULL, &timeoutValue)) < 0) {
        throw SocketException("Selector error");
    }
    
    // Filter the sets
    for (Set::iterator it = reads.begin(); it != reads.end(); ) {
        if (FD_ISSET((*it)->handle(), &readSet)) {
            // keep the socket in the set
            it++;
        } else {
            // remove the socket
            reads.erase(it++);
        }
    }
    
    for (Set::iterator it = writes.begin(); it != writes.end(); ) {
        if (FD_ISSET((*it)->handle(), &writeSet)) {
            // keep the socket in the set
            it++;
        } else {
            // remove the socket
            writes.erase(it++);
        }
    }
    
    // Result is 0 on timeout.
    // We want to return true when we have something to read/write.
    return (result > 0);
}