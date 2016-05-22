#include "perceptron.h"


int main()
{
	perceptron myNet;
	myNet.initFiles("input.bin", "output.bin", "bkp.bin");

	myNet.initNetwork();

	//myNet.writeWeightsToFile();
	myNet.readWeightsFromFile();
	myNet.getInput(0);

	myNet.processData();

	return 0;
	
}
