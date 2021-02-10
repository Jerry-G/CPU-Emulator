CXX=g++
CFLAGS=-g -lSDL2 -lSDL2_image --std=c++17
OBJS=main.o jm.o

all: jm.o main.o main

jm.o: jm.cpp
	$(CXX) -g --std=c++17 -o $@ -c $^

main.o: main.cpp
	$(CXX) -g --std=c++17 -o $@ -c $^

main: $(OBJS)
	$(CXX) -o $@ $(OBJS) $(CFLAGS)

clean:
	rm -f $(OBJS) main

