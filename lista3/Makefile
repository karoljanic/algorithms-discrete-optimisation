CC := g++
CFLAGS := -std=c++20 -Wall -g -O3

all: dijkstra dial radix

dijkstra:
	$(CC) $(CFLAGS) dijkstra.cpp Graph.cpp -o dijkstra

dial:
	$(CC) $(CFLAGS) dial.cpp Graph.cpp -o dial

radix:
	$(CC) $(CFLAGS) radix.cpp Graph.cpp -o radix

clean:
	rm -f dijkstra dial radix