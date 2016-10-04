#define LAYER_DEBUG 0

#ifdef GLOBAL_DEBUG
#define LAYER_DEBUG 1
#endif

#include "neuron_p.h"

class layer{
private:
	int numberOfNeurons;
	int inputVectorSize;

	vector<neuron> neurons;
public:
	vector<float> input;
	vector<float> output;

	layer();
	~layer();

	void initLayer(int neuronCount, int inputSize);
	void constructNeurons(bool isRand, const char *fname = NULL);

	void computeOutput();

	bool writeNeuronsToFile(FILE *fp);
	bool readNeuronsFromFile(FILE *fp);

	bool getNeuronWeight(int neuronNumber, int weightNumber, float *result);
	float getNeuronWeight_NV(int neuronNumber, int weightNumber);

	bool setNeuronWeights(int neuronNumber, vector<float> &newWeights);

	bool deltaNeuronWeights(int neuronNumber, float delta); /*< Change the weights of the neuron by delta */

	void getInput(vector <float> layerInput);
	vector<float> getOutput();

	int getNeuronCount();
	int getNeuronWeightCount();
	
};

