layer::layer()
{
	
}

layer::~layer()
{
	
}

void layer::initLayer(int neuronCount, int inputSize)
{
	/* Log here */
	numberOfNeurons = neuronCount;
	inputVectorSize = inputSize;
	
	input.resize(inputsCount);
	output.resize(numberOfNeurons);

}
void constructNeurons(bool isRand, const char *fname = NULL)
{
	/* LOG HERE */

	/**
	 * Initialize all the neurons with the isRand variable
	 * neuron(isRand) -- a call to the constructor with an 
	 * optional bool parameter
	 */
	for(int i = 0; i < neuronCount; i++) {
		neurons.push_back(neuron(isRand));

		/**
		 * The number of weight equals to the size
		 * of the input vector
		 */
		neurons[i].initNeuron(inputVectorSize);

		/* Log here */
	}

	/**
	 * if the optional argument is present -- read neurons from file
	 */
	if(fname != NULL) {
		readNeuronsFromFile(fname);
	}

}

void computeOutput()
{
	/* Log here */

	for(int i = 0; i < numberOfNeurons; i++) {
		neurons[i].summate(input);
		output[i] = neurons[i].output;
	}

}

bool writeNeuronsToFile(const char *fname);
bool readNeuronsFromFile(const char *fname);
