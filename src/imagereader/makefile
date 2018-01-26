read: main.o image_read.o
	g++ -Wall -std=c++11 -g main.o image_read.o -o read `pkg-config opencv --cflags --libs`
main.o: main.cc
	g++ -Wall -std=c++11 $(shell pkg-config --cflags opencv) -c main.cc
image_read.o: image_read.cc
	g++ -Wall -std=c++11 $(shell pkg-config --cflags opencv) -c image_read.cc
