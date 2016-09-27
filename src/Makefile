# Define compiler
CC=g++

# Default CFLAGS
CFLAGS=-g -std=c++11 -Wall -Wextra -pedantic -Weffc++ -Wold-style-cast -Wconversion -Wsign-conversion -Winit-self -Wunreachable-code

all: p


p: main_p.o neuron_p.o layer_p.o perceptron.o  
	$(CC) $(CFLAGS) neuron_p.cpp layer_p.cpp perceptron.cpp main_p.cpp -o p

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
