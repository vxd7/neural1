#include "perceptron.h"


int main(int argc, char *argv[])
{
	/* first argument -- programname */
	if(argc == 1) {
		cout<<"Parameters are needed.\n";
		cout<<"p.bin inputFile outputFile BKPFile learn_iters learn_velocity\n";

		exit(1);
	}

	string iFname(argv[1]), oFname(argv[2]), bkpFname(argv[3]);

	perceptron myNet;

	myNet.initFiles(iFname, oFname, bkpFname);

	myNet.initNetwork();

	/*myNet.readWeightsFromFile();

	myNet.getInput(0);
	myNet.processData();*/
	
	myNet.readWeightsFromFile();

	for(int i = 0; i < 10; i++) {
		myNet.getInput(i);
		/* do not write to file */
		myNet.processData();
	}
	
	myNet.printVectorsFromFile("output");
	myNet.eraseOutputFile();

	
	


	return 0;
	
}
