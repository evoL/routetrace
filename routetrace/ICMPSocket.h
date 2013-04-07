// Rafał Hirsz, nr 247955

#ifndef __routetrace__ICMPSocket__
#define __routetrace__ICMPSocket__

#include "Socket.h"
#include "ICMPPacket.h"

class ICMPSocket : public Socket {
public:
    ICMPSocket();
    
    virtual void send(const Packet& packet, std::string address);
    virtual void send(const ICMPPacket& packet, std::string address);
    
    ICMPPacket receive();
};

#endif /* defined(__routetrace__ICMPSocket__) */
