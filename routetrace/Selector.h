// Rafał Hirsz, nr 247955

#ifndef __routetrace__Selector__
#define __routetrace__Selector__

#include <set>
#include <sys/select.h>
#include "Socket.h"

class Selector {
public:
    typedef std::set<const Socket*> Set;
    
    Selector();
    ~Selector();
    
    void read(const Socket& s);
    void write(const Socket& s);
    void clear();
    
    void timeout(time_t sec);
    inline void infinite(bool inf = true) { waitInfinitely = inf; }
    
    inline const Set& availableToRead() const { return reads; }
    inline const Set& availableToWrite() const { return writes; }
    
    bool run();
private:
    Set reads, writes;
    timeval timeoutValue;
    time_t timeoutSeconds;
    bool waitInfinitely;
    int maxFd;
    
    fd_set readSet, writeSet;
};

#endif /* defined(__routetrace__Selector__) */
