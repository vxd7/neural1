#include "perceptron.h"

perceptron::perceptron(bool rnd /*= false*/)
{
	networkLayers = NULL;
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

	layerCount = 0;

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

bool perceptron::initNetwork(int newLayerCount, vector <int> layers)
{
	/**
	 * Initialize the number of layers
	 */
	layerCount = newLayerCount;
	networkLayers = new layer[layerCount];

	/**
	 * Initialize the neurons in layer array
	 */
	neuronInLayer = layers;
	
	cout<<"Input the number of components in the input vector\n"; cin>>inputVectorSize;
	pnInput.resize(inputVectorSize);

	/**
	 * Layers are indexed from ZERO
	 */
	outputVectorSize = layers[layerCount - 1];
	pnOutput.resize(outputVectorSize);

	/* Construct layers */
	randomize = true; /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/

	networkLayers[0].initLayer(neuronInLayer[0], inputVectorSize);
	networkLayers[0].constructNeurons(randomize);
	for(int i = 1; i < layerCount; i++) {
		networkLayers[i].initLayer(neuronInLayer[i], neuronInLayer[i - 1]);
		networkLayers[i].constructNeurons(randomize);
	}

	/**
	 * Get file names
	 */
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
	for(int i = 0; i < inputVectorSize; i++) {
		cout << i << "'th component: ";
		cin >> pnInput[i];
	}
	cout << "All done!" << endl;

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
	
	cout<<"Computing output...\n";

	cout << "----------LAYER 0 ----------" << endl;
	networkLayers[0].getInput(pnInput);
	networkLayers[0].computeOutput();

	for(int i = 1; i < layerCount; i++) {
		cout << "----------LAYER " << i << "----------" << endl;
		networkLayers[i].getInput(networkLayers[i - 1].getOutput());
		networkLayers[i].computeOutput();
	}

	pnOutput = networkLayers[layerCount - 1].getOutput();

	cout<<"Writing to the file...\n";
	writeVectorToFile(pnOutputFile, pnOutput);

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
	cout<<"\n-----Resulting output vector:-----" << endl;
	cout << "{";
	for(int i = 0; i < pnOutput.size(); i++) {
		cout << pnOutput[i] << ", ";
	}
	cout << "\b\b"; //cursor two positions backwards
	cout << "}" << endl;
	//readVectorFromFile(pnOutputFile, outputVectorCount - 1, sizeof(float) * outputVectorSize, true);


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

	//Set up multiple hard coded bkp files for multilayer
	//
	//
	//if(pnBkpFile == NULL) {
	//	/* Log here */
	//	cout<<"Backup file was not set properly!!\n";
	//	exit(1);
	//}
	
	string bkpFileNameAlias("layer");

	FILE *pnBkpLayerFile = NULL;
	for(int i = 0; i < layerCount; i++) {
		string tmpFileName = bkpFileNameAlias + to_string(i);

		if( (pnBkpLayerFile = fopen( tmpFileName.c_str(), "wb+")) == NULL) {
			cout << "ERROR opening file " << tmpFileName << endl;
		}

		if( networkLayers[i].writeNeuronsToFile(pnBkpLayerFile) ) {
			/* Log success here */
			cout<<"Successfully written weights: " << tmpFileName << endl;
		} else {
			/* Log failure here */
			cout<<"ERROR while writing weights to file" << tmpFileName << endl;
		}

		fclose(pnBkpLayerFile);
	}
}

void perceptron::readWeightsFromFile()
{
	bool success = true;

	//if(pnBkpFile == NULL) {
	//	/* Log failure here */
	//	cout<<"ERROR Backup file was not set\n";
	//	exit(1);
	//}
	
	string bkpFileNameAlias("layer");

	FILE *pnBkpLayerFile = NULL;
	for(int i = 0; i < layerCount; i++) {
		string tmpFileName = bkpFileNameAlias + to_string(i);

		if( (pnBkpLayerFile = fopen( tmpFileName.c_str(), "rb")) == NULL) {
			cout << "ERROR opening file " << tmpFileName << endl;
		}

		success = networkLayers[i].readNeuronsFromFile(pnBkpLayerFile);

		if(!success) {
			cout<<"ERROR while reading weights\n";
			break;
		}

		fclose(pnBkpLayerFile);
	}

	cout<<"Weights have been successfully read from file!\n";
	
}

// IS NOT NEEDED FOR MULTILAYER FOR NOW
// TODO: REWRITE LATER
//void perceptron::readWeightsFromFile(string newBkpFilename)
//{
//	bkpFileName	= newBkpFilename;
//
//	/**
//	 * If the file was _not_ openede previously
//	 * we need to open it first using supplied newBkpFilename
//	 * The file must exist -- user ensures it.
//	 */
//	if(pnBkpFile == NULL) {
//
//		if((pnBkpFile = fopen(bkpFileName.c_str(), "rb")) == NULL) {
//			/* Log error here */
//
//			cout<<"ERROR opening the file "<<bkpFileName;
//
//			exit(1);
//		}
//		
//	}
//	/**
//	 * If the file was actually opened previously (supposedly by initFiles)
//	 * we need to reopen this stream with another filename
//	 */
//	else {
//
//		/**
//		 * The file must exist -- "rb"
//		 */
//		if( (pnBkpFile = freopen(bkpFileName.c_str(), "rb", pnBkpFile)) == NULL) {
//			/* Log error here */
//			cout<<"ERROR preparing file to store weights in!\n";
//			exit(1);
//		}
//	}
//
//	/**
//	 * And call standart reading function when we are done with files
//	 */
//	readWeightsFromFile();
//	
//	
//}

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
			// TODO: Solve problem with BKP file
			fp = pnBkpFile; 

			//fileVectorCount = neuronCount;
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

void perceptron::eraseOutputFile()
{
	pnOutputFile = freopen(NULL, "wb+", pnOutputFile);
}

void perceptron::backpropLearn(vector <float> desiredOutput, float learningSpeed, int iterCount)
{
	/**
	 * ===== First we run backprop algo for the neurons of the output layer =====
	 */

	/**
	 * IMPORTANT: Last layer is the layer with the number of (layerCount - 1)
	 */

	int outLayerNeuronCount = networkLayers[layerCount - 1].getNeuronCount();
	int outLayerNeuronWeights = networkLayers[layerCount - 1].getNeuronWeightCount();
	float a = 0.9;

	vector <float> lGrads_outLayer;
	lGrads_outLayer.resize(outLayerNeuronCount);

	for(int i = 0; i < outLayerNeuronCount; i++) {
		lGrads_outLayer[i] = lGradOutNeuron(i, desiredOutput[i], a);
	}

	/**
	 * And change the weights of the output layer neurons
	 */

	float newOutNeuronDelta;

	for(int i = 0; i < outLayerNeuronCount; i++) {
		newOutNeuronDelta = learningSpeed * lGrads_outLayer[i] * networkLayers[layerCount - 1].output[i];

		networkLayers[layerCount - 1].deltaNeuronWeights(i, newOutNeuronDelta);

	}


	/**
	 * ===== Run backprop for hidden layers =====
	 */
	vector <float> oldLocalGrads;
	vector <float> newLocalGrads;

	oldLocalGrads = lGrads_outLayer;

	/**
	 * For each layer
	 */
	for(int i = 1; i < layerCount; i++) {
		int hLayerNeuronCount = networkLayers[i].getNeuronCount();
		int hLayerNeuronWeights = networkLayers[i].getNeuronWeightCount();

		newLocalGrads.clear();
		newLocalGrads.resize(hLayerNeuronCount);

		/**
		 * For each neuron in the layer we firstly compute its local gradient
		 */
		for(int j = 0; j < hLayerNeuronCount; j++) {
			newLocalGrads[j] = lGradHiddenNeuron(j, i, oldLocalGrads, a);
		}

		oldLocalGrads = newLocalGrads;

		/**
		 * And change the weights of the neurons in current hidden layer
		 */
		for(int j = 0; j < hLayerNeuronCount; j++) {
			float deltaHiddenNeuron = learningSpeed * newLocalGrads[j] * networkLayers[i].output[j];

			networkLayers[i].deltaNeuronWeights(j, deltaHiddenNeuron);
		}
	}

}

float perceptron::lGradOutNeuron(int neuronIndex, float desiredNeuronOut, float a)
{
	float neuronOut = networkLayers[layerCount - 1].output[neuronIndex];
	return a * (desiredNeuronOut - neuronOut) * neuronOut * (1.0 - neuronOut);
	
}

float perceptron::lGradHiddenNeuron(int neuronIndex, int currentLayerIndex, vector <float> nextLayerLocalGrads, float a)
{
	float nextLayerSum = 0.0;

	for(int i = 0; i < networkLayers[currentLayerIndex + 1].getNeuronCount(); i++) {
		nextLayerSum += nextLayerLocalGrads[i] * networkLayers[currentLayerIndex + 1].getNeuronWeight_NV(i, neuronIndex);
	}

	float neuronOut = networkLayers[currentLayerIndex].output[neuronIndex];

	return a * neuronOut * (1.0 - neuronOut) * nextLayerSum;

}
