#include "perceptron.h"


int main(int argc, char *argv[])
{
	perceptron myNet;
	myNet.initFiles("input", "output");

	vector <int> l;
	l.resize(2);

	l[0] = 5;
	l[1] = 4;

	myNet.initNetwork(2, l);

	myNet.getInput();
	myNet.processData();

	return 0;
	
}
