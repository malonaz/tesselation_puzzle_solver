read: main.o image_read.o
	g++ -Wall -g main.o image_read.o -o read `pkg-config opencv --cflags --libs`
main.o: main.cc
	g++ -Wall -c main.cc
image_read.o: image_read.cc
	g++ -Wall -c image_read.cc
