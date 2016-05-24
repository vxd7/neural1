#include "perceptron.h"

perceptron::perceptron(bool rnd /*= false*/)
{
	/**
	 * Generate the seed for the weights randomiation
	 */
    srand (static_cast <unsigned> (time(0)));

	/**
	 * Init all the files pointers
	 */
	pnInputFile = NULL;
	pnOutputFile = NULL;
	pnBkpFile = NULL;

	/**
	 * Init all the vectorCount vars
	 */
	inputVectorCount = 0;
	outputVectorCount = 0;

	/**
	 * Init all the vectorSize vars
	 */
	inputVectorSize = 0;
	outputVectorSize = 0;

	/**
	 * Init all the flags and flag variables
	 */
	randomize = rnd;
	filesInitFlag = false;
}

perceptron::~perceptron()
{
	if(pnInputFile != NULL)
		fclose(pnInputFile);

	if(pnOutputFile != NULL)
		fclose(pnOutputFile);

	if(pnBkpFile != NULL)
		fclose(pnBkpFile);
	
}

bool perceptron::initNetwork()
{
	
	cout<<"Input the number of components in the input vector\n"; cin>>inputVectorSize;
	pnInput.resize(inputVectorSize);

	cout<<"Number of neurons\n"; cin >> neuronCount;
	pnOutput.resize(neuronCount);

	/*
	 * Number of components in the output vector equals to the 
	 * number of neurons in the layer
	 */
	outputVectorSize = neuronCount;

	/* Construct a layer */
	pnLayer.initLayer(neuronCount, inputVectorSize);
	randomize = true; /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

	if(!filesInitFlag) {
		string tmpInputFilename, tmpOutputFilename, tmpBkpFilename;

		cout<<"Input files initialization information:\n";
		cout<<"Input the name of input file: "; cin>>tmpInputFilename;
		cout<<"Input the name of output file (optional): "; cin>>tmpOutputFilename;
		cout<<"Input the name of backup file to store weights in (optional): "; cin>>tmpBkpFilename;

		initFiles(tmpInputFilename, tmpOutputFilename, tmpBkpFilename);
	}

	cout<<"inputVectorSize: "<<inputVectorSize<<"\n";
	inputVectorCount = getComponentCount(pnInputFile, sizeof(float))/inputVectorSize;
	outputVectorCount = getComponentCount(pnOutputFile, sizeof(float))/outputVectorSize;

	pnLayer.constructNeurons(randomize);

	return true;
}

bool perceptron::initFiles(string inputFile, string outputFile /*= ""*/, string bkpFile /*= ""*/)
{
	/**
	 * Set the flag -- files are considered initialized now 
	 */
	filesInitFlag = true;

	inputFileName = inputFile;
	outputFileName = outputFile;
	bkpFileName = bkpFile;

	/* Create files if necessary: */
	if((pnInputFile = fopen(inputFileName.c_str(), "ab+")) == NULL) {
		/* Log error here */
		exit(1);
	}
	//fclose(pnInputFile);

	if(outputFileName != "") {

		/**
		 * Create when necessary
		 */
		if((pnOutputFile = fopen(outputFileName.c_str(), "ab+")) == NULL) {
			/* Log error here */
			exit(1);
		}
		//fclose(pnOutputFile);
		
	}

	if(bkpFileName != "") {

		/**
		 * Create when necessary
		 */
		if((pnBkpFile = fopen(bkpFileName.c_str(), "ab+")) == NULL) {
			/* Log error here */
			exit(1);
		}
		//fclose(pnBkpFile);
	}

	/**
	 * Get the number of vectors in the given files
	 */

	return true;
}

void perceptron::getInput()
{
}

/**
 * Read k'th input vector from file, store it in the vector<float> input
 * k is indexed from 0
 */
void perceptron::getInput(int k)
{
	/**
	 * Check whether we have correctly opened descriptor in our class
	 * If no -- there may be a serious error
	 */
	if(pnInputFile == NULL) {
		/* Log error here */
		exit(1);
	}

	if(inputVectorCount == 0) {
		/* Log error */
		cout<<"ERROR input file is empty!";
		exit(1);
	}

	/**
	 * Components of the input vector are indexed FROM 0.
	 */
	if(k > (inputVectorCount - 1)) {
		exit(1);
	}

	fseek(pnInputFile, sizeof(float) * inputVectorSize * k, SEEK_SET);

	cout<<"+++Read vector from a file:+++\n";
	for(int i = 0; i < inputVectorSize; i++) {
		fread(&pnInput[i], sizeof(float), 1, pnInputFile);
		cout<<"Vector "<<k<<" component "<<i<<": "<<pnInput[i]<<"\n";
	}
	
}

/**
 * Get vec'th vector's comp'th component.
 * vec and comp are indexed from 0
 */
float perceptron::getInputVectorComponent(int vec, int comp)
{
	float component;

	/* Check whether given values are correct. (-1) because vec and comp are indexed from 0 */
	if(comp > (inputVectorSize - 1) ||(  vec > ( getComponentCount(pnInputFile, sizeof(float))/inputVectorSize - 1 )  )) {

		/* ErrLog here */
		exit(1);

	}
	/* Jump directly to the vec'th vector*/
    fseek(pnInputFile, (sizeof(float) * inputVectorSize * vec), SEEK_SET);

	/* Jump to the comp'th component of the vec'th vector */
	fseek(pnInputFile, (comp * sizeof(float)), SEEK_CUR);

	/* Read the needed vector's component */
	fread(&component, sizeof(float), 1, pnInputFile);

	return component;
}



/**
 * Get vec'th vector's comp'th component.
 * vec and comp are indexed from 0
 *
 * We assume that outputVectorCount variable is initialized by now
 */
float perceptron::getOutputVectorComponent(int vec, int comp)
{
	float component;

	/* Check whether given values are correct. (-1) because vec and comp are indexed from 0 */
	if((vec > outputVectorCount - 1) || (comp > outputVectorSize - 1))
	{
		/* Error log here */
		exit(1);

	}
	/* Jump directly to the vec'th vector in the input file */
    fseek(pnOutputFile, (sizeof(float) * outputVectorSize * vec), SEEK_SET);

	/* Jump to the comp'th component of the vec'th vector */
	fseek(pnOutputFile, (comp * sizeof(float)), SEEK_CUR);

	/* Read the needed vector's component */
	fread(&component, sizeof(float), 1, pnOutputFile);

	return component;

}

void perceptron::processData(bool write /* = true */)
{
	
	for(int i = 0; i < inputVectorSize; i++) { 
		pnLayer.input[i] = pnInput[i];
	}

	cout<<"Computing output...\n";
	pnLayer.computeOutput();

	for(int i = 0; i < neuronCount; i++) {
		pnOutput[i] = pnLayer.output[i];
	}

	cout<<"Writing to the file...\n";
	writeVectorToFile(pnOutputFile, pnOutput, neuronCount);

	/**
	 * And change the outputVectorCount variable
	 * because we have written something to the output file by now
	 */
	cout<<"outputVectorSize: "<<outputVectorSize<<"\n";
	outputVectorCount = getComponentCount(pnOutputFile, sizeof(float))/outputVectorSize;

	/**
	 * Read written vector -- it starts with the (outputVectorCount - 1)'th vector
	 * true because we want to output to STDOUT
	 */
	cout<<"\n-----Resulting output vector:-----\n";
	readVectorFromFile(pnOutputFile, outputVectorCount - 1, sizeof(float) * outputVectorSize, true);


}

/**
 * Simply writes given array to the file
 */
bool perceptron::writeVectorToFile(FILE *fp, vector<float> &arr, int n /*= -1*/)
{
	if(fp == NULL) {
		/* Log error here */

		cout<<"Invalid output file! Function writeVectorToFile\n";
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	
	/* Log here*/ 

//	if(n == -1) n = getComponentCount(fp, sizeof(float));

	cout<<arr.size()<<"\n";
	int check = 0;
	for(int i = 0; i < arr.size(); i++) {
		cout<<"ARR["<<i<<"] is: "<<arr[i]<<"\n";
		check = fwrite(&arr[i], sizeof(float), 1, fp);

		if(check != 1) {
			cout<<"ERROR writing arr["<<i<<"] to file!\n";
			exit(1);
			
		}
	}

	/* Write everything without closing file */
	if(fflush(fp) != 0) cout<<"ERROR writeVectorToFile\n";

	return true;
}

/**
 * Read vectors from file starting with number `start' and ending with number `end' INCLUSIVELY
 * fp -- pointer to the file to read from ;
 * start -- starting vector, indexes from 0 ;
 * vectorSize -- total number of (FLOAT) components in the vector in bytes
 * print -- whether we want to print everything to STDOUT.
 */
vector<float> perceptron::readVectorFromFile(FILE *fp, int start, size_t vectorSize, bool print)
{

	vector<float> tmp;
	float comp;
	int componentCount = vectorSize/sizeof(float);

	if(fp == NULL) {
		/* Log here */
		cout<<"Invalid file pointer to read from! Function readVectorFromFile\n";
		exit(1);
	}
	
	/**
	 * Check if everything is correct
	 */
	if(start < 0) {
		/* Log here */

		cout<<"ERROR: entered values are incorrect! Function readVectorFromFile\n";
		exit(1);
	}

	/* One more chacking is needed -- whether we are reading inexistend data */
	
	fseek(fp, vectorSize * start, SEEK_SET);
		

	for(int i = 0; i < componentCount; i++) {

		fread(&comp, sizeof(float), 1, fp);
		cout<<"Read component #"<<i<<" is: "<<comp<<"\n";
		tmp.push_back(comp);

	}

	if(print) {
		cout << "{";
		for(int i = 0; i < tmp.size(); i++) {
			cout << tmp[i] << ", ";
		}
		cout << "\b\b" << "}"; //'\b' -- one position backwards
		cout<<"\n";
	}

	return tmp;
	
}
void perceptron::writeWeightsToFile()
{

	if(pnBkpFile == NULL) {
		/* Log here */
		cout<<"Backup file was not set properly!!\n";
		exit(1);
	}

	/**
	 * Reopen file with "wb+" in order to erase any content there.
	 */
	if( (pnBkpFile = freopen(NULL, "wb+", pnBkpFile)) == NULL) {
		/* Log error here */
		cout<<"ERROR preparing file to store weights in!\n";
		exit(1);
	}

	if( pnLayer.writeNeuronsToFile(pnBkpFile) ) {
		/* Log success here */
		cout<<"Successfully written weights\n";
	}
	else {
		/* Log failure here */
		cout<<"ERROR while writing weights to file\n";
	}
}

void perceptron::readWeightsFromFile()
{
	bool success = true;

	if(pnBkpFile == NULL) {
		/* Log failure here */
		cout<<"ERROR Backup file was not set\n";
		exit(1);
	}

	success = pnLayer.readNeuronsFromFile(pnBkpFile); 

	if(!success) {
		cout<<"ERROR while reading weights\n";
		exit(1);
	}

	cout<<"Weights have been successfully read from file!\n";
	
}

void perceptron::readWeightsFromFile(string newBkpFilename)
{
	bkpFileName	= newBkpFilename;

	/**
	 * If the file was _not_ openede previously
	 * we need to open it first using supplied newBkpFilename
	 * The file must exist -- user ensures it.
	 */
	if(pnBkpFile == NULL) {

		if((pnBkpFile = fopen(bkpFileName.c_str(), "rb")) == NULL) {
			/* Log error here */

			cout<<"ERROR opening the file "<<bkpFileName;

			exit(1);
		}
		
	}
	/**
	 * If the file was actually opened previously (supposedly by initFiles)
	 * we need to reopen this stream with another filename
	 */
	else {

		/**
		 * The file must exist -- "rb"
		 */
		if( (pnBkpFile = freopen(bkpFileName.c_str(), "rb", pnBkpFile)) == NULL) {
			/* Log error here */
			cout<<"ERROR preparing file to store weights in!\n";
			exit(1);
		}
	}

	/**
	 * And call standart reading function when we are done with files
	 */
	readWeightsFromFile();
	
	
}

int perceptron::getComponentCount(FILE* fp, float component_size)
{
	/**
	 * Check whether pointer is correct
	 */
	if(fp == NULL)
		return -1;

	int num, pos;
	pos = ftell(fp); //save our old position in file 

	fseek(fp, 0, SEEK_END);
	num = (int)(ftell(fp)/component_size);
	fseek(fp, pos, SEEK_SET); //restore our old position

	return num;
	
}

void perceptron::printVectorsFromFile(string pnFileType)
{

	FILE *fp;

	int fileVectorCount;
	int fileVectorCompCount;

	map<string, int> typeMap; 

	typeMap["input"] = 1;
	typeMap["output"] = 2;
	typeMap["bkp"] = 3;
						
	switch(typeMap[pnFileType]) {
		case 1:
			fp = pnInputFile; 

			fileVectorCount = inputVectorCount;
			fileVectorCompCount = inputVectorSize;

			break;
		case 2:
			fp = pnOutputFile; 

			fileVectorCount = outputVectorCount;
			fileVectorCompCount = outputVectorSize;

			break;
		case 3:
			fp = pnBkpFile; 

			fileVectorCount = neuronCount;
			fileVectorCompCount = inputVectorSize;

			break;
		default:
			cout<<"Error: no such file type!\n";
			return;
			break;
	}

	printVectorsFromFile(fp, fileVectorCount, fileVectorCompCount);
	
}
void perceptron::printVectorsFromFile(FILE *fp, int fileVectorCount, int fileVectorCompCount)
{
	fseek(fp, 0, SEEK_SET);

	for(int i = 0; i < fileVectorCount; i++) {

		fseek(fp, i * fileVectorCompCount * sizeof(float), SEEK_SET);
		cout<<"Vector #"<<i<<": {";

		for(int j = 0; j < fileVectorCompCount; j++) {
			float comp;
			fread(&comp, sizeof(float), 1, fp);
			cout<<comp<<", ";
		}
		cout<<"\b\b}\n";
	
	}
}

/* CAUTION!! CRAPPY CODE!
 * TODO: Rewrite fukken everything in learn functions
 */
/**
 * range -- the total number of vectors to read from the start of the file
 * n -- the velocity of learning
 */
void perceptron::learn_digits(string idealOutput, int range, float n)
{
	                 /*    0         1          2          3          4          5          6          7          8          9    */
	int ans[10][4] = { {0,0,0,0}, {0,0,0,1}, {0,0,1,0}, {0,0,1,1}, {0,1,0,0}, {0,1,0,1}, {0,1,1,0}, {0,1,1,1}, {1,0,0,0}, {1,0,0,1}};

	FILE *idealOutputFile;
	/**
	 * ideal output Vector Count
	 * The number of vectors in the idealOutput file
	 */
	int iVectorCount;

	/**
	 * ideal Output Component Count
	 * The number of components (FLOAT) in the idealOutput file
	 */
	int iComponentCount;

	/**
	 * delta = T - OUT
	 * T -- ideal output
	 * OUT -- perceptron output
	 */
	float delta;

	if( (idealOutputFile = fopen(idealOutput.c_str(), "rb")) == NULL) {
		/* Log error here -- critical failure */

		cout<<"Error reading ideal learning output vectors!\n";
		exit(1);
	}

	iComponentCount = getComponentCount(idealOutputFile, sizeof(float));

	/**
	 * The number of output components in learning vectors _must_ be 
	 * equal to the number of output components in the output file vectors
	 */
	iVectorCount = iComponentCount/outputVectorSize;

	cout<<"iComponentCount: "<<iComponentCount<<"\n";
	cout<<"outputVectorSize: "<<outputVectorSize<<"\n";

	cout<<"iVectorCount: "<<iVectorCount<<"\n";
	cout<<"outputVectorCount: "<<outputVectorCount<<"\n";
	cout<<"inputVectorCount: "<<inputVectorCount<<"\n";

	if((range > iVectorCount) || (range > outputVectorCount) || (range > inputVectorCount)) {
		/* Log failure here */

		cout<<"Incorrect range parameter! Function learn_digits(...)";
		exit(1);
	}

	if(pnInputFile == NULL) {
		/* Log failure */
		cout<<"Cannot access input learning samples\n";
		exit(1);
	}
	
	float oldWeight;;

	vector<float> idealOut;
	float realOutComp;
	float inputVectorComp;

	float weightChange;

	cout<<"\n+_+_+_+_+_+_STARTING LEARNING ALGO:_+_+_+_+_+_+_+\n";

	/* For every output vector... */
	for(int  i = 0; i < range; i++) {
		cout<<"Output vector number: "<<i<<"\n";

		cout<<"Reading ideal output file...\n";
		idealOut = readVectorFromFile(idealOutputFile, i, sizeof(float) * outputVectorSize, false);

		/* go through it's components */
		for(int j = 0; j < outputVectorSize; j++) {
			cout<<"Output vector component(NEURON No): "<<j<<"\n";
			vector<float> newWeights;

			realOutComp = getOutputVectorComponent(i, j);
			delta = idealOut[j] - realOutComp;

			/* Every component of output vector is one neuron.
			 * Go through the weights of this j'th neuron */
			for(int k = 0; k < inputVectorSize; k++) {
				cout<<"NEURON: "<<j<<": COMP: "<<k<<"\n";

				if(delta == 0) {
					cout<<"Neuron is stable. \n";
					break;
				}
				/**
				 * j'th output component -- j'th neuron
				 */

				/**
				 * For i'th input vector get it's k'th component
				 * k -- ordinary number of neuron's current weight
				 */
				inputVectorComp = getInputVectorComponent(i, k);
				cout<<"inputVectorComp: "<<inputVectorComp<<"\n";

				pnLayer.getNeuronWeight(j, k, &oldWeight);
				weightChange = oldWeight + n * delta * inputVectorComp;

				cout<<"WEIGHT CHANGE: "<<weightChange<<"\n";

				/**
				 * Accumulate every weight's change -- write it to the vector
				 */
				newWeights.push_back(weightChange);
			}

			/**
			 * And finally store weights in the neuron
			 */
			if(delta != 0) {
				pnLayer.setNeuronWeights(j, newWeights);
				cout<<"WRITTEN\n";
			}
		}
	}

	fclose(idealOutputFile);
}

void perceptron::eraseOutputFile()
{
	pnOutputFile = freopen(NULL, "wb+", pnOutputFile);
}
