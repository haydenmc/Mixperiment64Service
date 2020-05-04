CC=g++
RM=rm -f
CFLAGS=-g -std=c++17 $(shell pkg-config --cflags gstreamer-1.0 tinyxml2)
LDFLAGS=-g -pthread
LDLIBS=$(shell pkg-config --libs gstreamer-1.0 tinyxml2) -lstdc++fs

SRCS=Main.cpp Mixperiment64.cpp Streamer.cpp MupenManager.cpp Config.cpp NodeManager.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

mixperiment64: $(OBJS)
	$(CC) $(LDFLAGS) -o mixperiment64 $(OBJS) $(LDLIBS)

all: mixperiment64

clean:
	rm -f $(OBJS) mixperiment64