CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		follow.o

LIBS =      /usr/local/Aria/lib/libAria.so

TARGET =	follow

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)



all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
