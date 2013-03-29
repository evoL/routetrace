//
//  Packet.h
//  routetrace
//
//  Created by Rafał Hirsz on 29.03.2013.
//  Copyright (c) 2013 Rafał Hirsz. All rights reserved.
//

#ifndef __routetrace__Packet__
#define __routetrace__Packet__

class Packet {
public:
    virtual inline int length() const = 0;
    virtual const void* structure() const = 0;
};

#endif /* defined(__routetrace__Packet__) */
