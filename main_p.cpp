#include "perceptron.h"


int main()
{
	perceptron myNet;
	myNet.initFiles("input.bin", "output.bin", "bkp.bin");

	myNet.initNetwork();

	myNet.writeWeightsToFile();
	//myNet.readWeightsFromFile();
	for(int i = 0; i < 10; i++) {
		myNet.getInput(i);
		myNet.processData();
		
	}

	myNet.learn_digits("iOut.bin", 9, 0.5);

	return 0;
	
}
