CXX = g++
CXXFLAGS = -Wall -W
TARGETS = routetrace
OBJECTS = Packet.o Socket.o Selector.o ICMPPacket.o IPPacket.o ICMPSocket.o main.o

all: $(TARGETS)

routetrace: $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@

clean:
	rm -f *.o

distclean: clean
	rm -f $(TARGETS)
