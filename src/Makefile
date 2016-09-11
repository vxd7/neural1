# Define compiler
CC=g++

# Default CFLAGS
CFLAGS=-g -c -Wall -Wextra -pedantic -Weffc++ -Wold-style-cast -Wconversion -Wsign-conversion -Winit-self -Wunreachable-code

all: p


p: main_p.o perceptron.o layer_p.o neuron_p.o
	$(CC) main_p.o perceptron.o layer_p.o neuron_p.o -o p


main_p.o: main_p.cpp
	$(CC) $(CFLAGS) main_p.cpp


perceptron.o: perceptron.cpp
	$(CC) $(CFLAGS) perceptron.cpp


layer_p.o: layer_p.cpp
	$(CC) $(CFLAGS) layer_p.cpp


neuron_p.o: neuron_p.cpp
	$(CC) $(CFLAGS) neuron_p.cpp


clean:
	rm *.o p
