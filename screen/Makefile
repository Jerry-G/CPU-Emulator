CXX=g++
CFLAGS=-O2 -lSDL2 -std=gnu++17

all: main

main: main.cpp
	$(CXX) -g -o $@ $^ $(CFLAGS)

clean:
	rm -f $(OBJS) main
