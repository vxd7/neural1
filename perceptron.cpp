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
	inputVectorCount = getComponentCount(pnInputFile, sizeof(float))/inputVectorSize;
	outputVectorCount = getComponentCount(pnOutputFile, sizeof(float))/outputVectorSize;

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

void perceptron::processData()
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
	 * Read written vectors from 0'th to the last one
	 * (neuronCount - 1) because 3'rd argument indexes from 0
	 * true because we want to output to STDOUT
	 */
	cout<<"\n-----Resulting output vector:-----\n";
	readVectorFromFile(pnOutputFile, 0, neuronCount - 1, true);


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
 * Read vectors from file starting with number `start' and ending with number `end' INCLUSIVE
 * fp -- pointer to the file to read from ;
 * start -- starting vector, indexes from 0 ;
 * end -- ending vector, indexes from 0 ;
 * print -- whether we want to print everything to STDOUT.
 */
vector<float> perceptron::readVectorFromFile(FILE *fp, int start, int end, bool print)
{

	vector<float> tmp;
	float comp;

	if(fp == NULL) {
		/* Log here */
		cout<<"Invalid file pointer to read from! Function readVectorFromFile\n";
		exit(1);
	}
	
	/**
	 * Check if everything is correct
	 */
	if( (start < 0) || (end > getComponentCount(fp, sizeof(float)) * sizeof(float) - 1) ) {
		/* Log here */

		cout<<"ERROR: entered values are incorrect! Function readVectorFromFile\n";
		exit(1);
	}

	if(end < start) {
		/* Log here */

		cout<<"ERROR1: entered values are incorrect! Function readVectorFromFile\n";
		exit(1);
	}
	
	fseek(fp, start * sizeof(float), SEEK_SET);
		

	/**
	 * <=end because indexation is from 0 and we are reading vectors inclusively */
	for(int i = start; i <= end; i++) {

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

/* CAUTION!! CRAPPY CODE!
 * TODO: Rewrite fukken everything in learn functions
 */
void perceptron::learn_digits()
{
	                 /*    0         1          2          3          4          5          6          7          8          9    */
	int ans[10][4] = { {0,0,0,0}, {0,0,0,1}, {0,0,1,0}, {0,0,1,1}, {0,1,0,0}, {0,1,0,1}, {0,1,1,0}, {0,1,1,1}, {1,0,0,0}, {1,0,0,1}};
}
