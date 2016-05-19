#include "perceptron.h"

perceptron::perceptron(bool rnd = false)
{
	/**
	 * Generate the seed for the weights randomiation
	 */
    srand (static_cast <unsigned> (time(0)));

	pnInputFile = NULL;
	pnOutputFile = NULL;
	pnBkpFile = NULL;

	randomize = rnd;

	inputVectorCount = 0;
	outputVectorCount = 0;

	inputVectorSize = 0;
	outputVectorSize = 0;
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
	pnLayer.constructNeurons(randomize);

	return true;
}

bool perceptron::initFiles(string inputFile, string outputFile = "", string bkpFile = "")
{
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
		 * Erase contents of the file if there is any.
		 * Create when necessary
		 */
		if((pnBkpFile = fopen(bkpFileName.c_str(), "wb+")) == NULL) {
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
	if(pnInput == NULL) {
		/* Log error here */
		exit(1);
	}

	/**
	 * Components of the input vector are indexed FROM 0.
	 */
	if(k > (inputVectorCount - 1)) {
		exit(1);
	}

	fseek(fp, sizeof(float) * inputVectorSize * k, SEEK_SET);

	for(int i = 0; i < inputVectorSize; i++) {
		fread(&pnInput[i], sizeof(float), 1, pnInput);
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
	if(comp > (inputVectorSize - 1) || (vec > (getComponentCount(pnInput, sizeof(float))/inputVectorSize - 1) {

		/* ErrLog here */
		exit(1);

	}
	/* Jump directly to the vec'th vector*/
    fseek(fp, (sizeof(float) * intputVectorSize * vec), SEEK_SET);

	/* Jump to the comp'th component of the vec'th vector */
	fseek(fp, (comp * sizeof(float)), SEEK_CUR);

	/* Read the needed vector's component */
	fread(&component, sizeof(float), 1, fp);

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
    fseek(fp, (sizeof(float) * outputVectorSize * vec), SEEK_SET);

	/* Jump to the comp'th component of the vec'th vector */
	fseek(fp, (comp * sizeof(float)), SEEK_CUR);

	/* Read the needed vector's component */
	fread(&component, sizeof(float), 1, fp);

	return component;

}

void perceptron::processData()
{
	
	for(int i = 0; i < inputVectorSize; i++) { 
		pnLayer.input[i] = pnInput[i];
	}

	pnLayer.computeOutput();

	for(int i = 0; i < neuronCount; i++) {
		pnOutput[i] = pnLayer.output[i];
	}

	writeVectorToFile(pnOutputFile, pnOutput, neuronCount);

	/**
	 * Read written vectors from 0'th to the last one
	 * (neuronCount - 1) because 3'rd argument indexes from 0
	 */
	readVectorFromFile(pnOutputFile, 0, neuronCount - 1, true);

}

/**
 * Simply writes given array to the file
 */
bool perceptron::writeVectorToFile(FILE *fp, vector<float> &arr, int n = -1)
{
	if(fp== NULL) {
		/* Log error here */
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	
	/* Log here*/ 

	if(n == -1) n = getComponentCount(fp, sizeof(float));

	for(int i = 0; i < n; i++) {
		fwrite(arr[i], sizeof(float), 1, fp);
	}

	/* Write everything without closing file */
	fflush(fp);

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
		exit(1);
	}
	
	/**
	 * Check if everything is correct
	 */
	if( (start < 0) || (end > getComponentCount(fp, sizeof(float)) * sizeof(float) - 1) ) {
		/* Log here */
		exit(1);
	}

	if(end > start) {
		/* Log here */
		exit(1);
	}

	/**
	 * <=end because indexation is from 0 and we are reading vectors inclusively */
	for(int i = start; i <= end; i++) {

		fread(&comp, sizeof(float), 1, fp);
		tmp.push_back(comp);
	}

	if(print) {
		cout << "{";
		for(int i = 0; i < tmp.size(); i++) {
			cout << tmp[i] << ", "
		}
		cout << '\b\b' << "}"; //'\b' -- one position backwards
		cout<<"\n";
	}

	return tmp;
	
}
void perceptron::writeWeightsToFile()
{

	if(pnBkpFile == NULL) {
		/* Log here */
		exit(1);
	}

	for(int i = 0; i < neuronCount; i++) {
	}
}

int perceptron::getComponentCount(FILE* fp, float component_size)
{
	int num, pos;
	pos = ftell(fp); //save our old position in file 

	fseek(fp, 0, SEEK_END);
	num = (int)(ftell(fp)/component_size);
	fseek(fp, pos, SEEK_SET); //restore our old position

	return num;
	
}
