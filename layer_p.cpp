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
	cout<<"\n";
	cout<<"LAYER: Started computing output...\n";

	for(int i = 0; i < numberOfNeurons; i++) {
		cout<<"Neuron #"<<i<<":\n";
		neurons[i].summate(input);
		output[i] = neurons[i].output;
		cout<<"\n\nNeuron #"<<i<<": OUTPUT: "<<output[i]<<"\n";
		
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

bool layer::getNeuronWeight(int neuronNumber, int weightNumber, float *result)
{
	bool success = true;

	if(neuronNumber > numberOfNeurons) {
		/* Log error */
		cout<<"Incorrect neuron number! Function getNeuronWeight\n";

		return false;
	}

	success = neurons[neuronNumber].getWeight(weightNumber, result);

	if(success)
		return true;
	else
		return false;

}

bool layer::setNeuronWeights(int neuronNumber, vector<float> &newWeights)
{
	bool success = true;

	if(neuronNumber > numberOfNeurons) {
		/* Log error */
		cout<<"Incorrect neuron number! Function setNeuronWeights\n";

		return false;
	}

	success = neurons[neuronNumber].changeWeights(newWeights);

	if(success)
		return true;
	else
		return false;

}
