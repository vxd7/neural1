#include "perceptron.h"


int main()
{
	perceptron myNet;
	myNet.initNetwork();

	myNet.initFiles("input.bin");

	return 0;
	
}
