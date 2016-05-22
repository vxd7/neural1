#include "layer_p.h"

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
	
	input.resize(inputVectorSize);
	output.resize(numberOfNeurons);

}
void layer::constructNeurons(bool isRand, const char *fname /*= NULL*/)
{
	/* LOG HERE */

	/**
	 * Initialize all the neurons with the isRand variable
	 * neuron(isRand) -- a call to the constructor with an 
	 * optional bool parameter
	 */
	for(int i = 0; i < numberOfNeurons; i++) {
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
		//readNeuronsFromFile(fname);
	}

}

void layer::computeOutput()
{
	/* Log here */

	for(int i = 0; i < numberOfNeurons; i++) {
		neurons[i].summate(input);
		output[i] = neurons[i].output;
	}

}

bool layer::writeNeuronsToFile(FILE *fp)
{
	bool success = true;

	for(int i = 0; i < numberOfNeurons; i++) { 

		if(!neurons[i].writeWeights(fp)) {
			success = false;
			break;
		}
	}

	if(success)
		return true;
	else
		return false;

}
bool layer::readNeuronsFromFile(FILE *fp)
{
	bool success = true;

	for(int i = 0; i < numberOfNeurons; i++) { 
		cout<<"Neuron ##"<<i<<":\n";
		if( !neurons[i].readWeights(fp, i) ) {
			success = false;
			break;
		}
	}

	if(success)
		return true;
	else
		return false;

}
