//
//  Packet.h
//  routetrace
//
//  Created by Rafał Hirsz on 29.03.2013.
//  Copyright (c) 2013 Rafał Hirsz. All rights reserved.
//

#ifndef __routetrace__Packet__
#define __routetrace__Packet__

#include <vector>

class Packet {
public:
    typedef std::vector<unsigned char> Data;
    
    Packet() {}
    Packet(Data data): rawData(data) {}
    
    virtual inline int length() const = 0;
    virtual const void* structure() const = 0;
    inline const Data& data() const { return rawData; }
protected:
    Data rawData;
};

#endif /* defined(__routetrace__Packet__) */
