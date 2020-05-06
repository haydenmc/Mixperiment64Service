CC=g++
RM=rm -f
CFLAGS=-std=c++17 $(shell pkg-config --cflags gstreamer-1.0 tinyxml2)
LDFLAGS=-pthread
LDLIBS=$(shell pkg-config --libs gstreamer-1.0 tinyxml2) -lstdc++fs

SRCS=Main.cpp Mixperiment64.cpp Streamer.cpp MupenManager.cpp Config.cpp NodeManager.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: CFLAGS+= -g
all: LDFLAGS+= -g
all: mixperiment64

release: CFLAGS+= -O3
release: LDFLAGS+= -O3
release: mixperiment64

mixperiment64: $(OBJS)
	$(CC) $(LDFLAGS) -o mixperiment64 $(OBJS) $(LDLIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) mixperiment64