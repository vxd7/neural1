#include "perceptron.h"


int main(int argc, char *argv[])
{
	/* first argument -- programname */
	if(argc == 1) {
		cout<<"Parameters are needed.\n";
		cout<<"p.bin inputFile outputFile BKPFile learn_iters learn_velocity ideal_Out\n";

		exit(1);
	}

	string iFname(argv[1]), oFname(argv[2]), bkpFname(argv[3]), idealOutFname(argv[4]);
	int N = atoi(argv[5]);
	float learn_vel = atof(argv[6]);

	cout<<N<<" iters will be made\n";
	cout<<learn_vel<<" is the learning velocity\n";


	perceptron myNet;

	myNet.initFiles(iFname, oFname, bkpFname);

	myNet.initNetwork();

	/*myNet.readWeightsFromFile();

	myNet.getInput(0);
	myNet.processData();*/
	
	for(int q = 0; q < N; q++){
		myNet.readWeightsFromFile();

		for(int i = 0; i < 20; i++) {
			myNet.getInput(i);
			myNet.processData();
			
		}
		
		myNet.learn_digits(idealOutFname, 20, learn_vel);
		myNet.writeWeightsToFile();

		/*cout<<"\n";
		cout<<"Learning is finished. Input new in filename\n";
		cin>>iFname;*/
		myNet.eraseOutputFile();

		for(int i = 0; i < 20; i++) {
			myNet.getInput(i);
			/* do not write to file */
			myNet.processData();
		}
		
		myNet.printVectorsFromFile("output");
		myNet.eraseOutputFile();
	}

	
	


	return 0;
	
}
