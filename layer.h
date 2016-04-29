#include "neuron.h"

/**
 * Class layer -- a set of neurons.
 * Being a set of neurons, layer has a number of inputs, 
 * number of neurons (usually not equal to number of inputs),
 * input and output vectors. Layer initializes its neurons,
 * assigns input vectors and takes outputs from each neuron in this layer.
 * <p>
 * Additionally layer is responsible for computing the beta variable, which is 
 * being used in the initialization process.
 * <p>
 * Layer also writes all its neurons' weights to file and reads this values when needed,
 */
class layer{
public:
    neuron *neurons; /**< The neurons in this layer */
    int numberOfNeurons; /**< Number of neurons in this layer */
    int inputsCount; /**< Number of neurons in previous layer, equals to the number of inputs to the current layer */

    float *inputs, *outputs; /**< Input and output vectors */

	/**
	 * A function for initialization of a layer. 
	 * Takes neuronsC and inputsC variables, assigned be the neuralNetwork class.
	 * <p>
	 * neuronsC - the number of neurons, needed in current layer <br>
	 * inputsC - the number of inputs, a size of an input vector to the current layer 
	 */
    void initLayer(int neuronsC, int inputsC);

	/**
	 * A constructor
	 * mmm
	 */
    layer();

	/** 
	 * A destructor.
	 * Frees memory, deleting the neurons in the current layer, deleting input and output vectors
	 */
    ~layer();

    float computeBeta();

    void constructNeurons(bool isRand, const char *fname);

    float *computeOutput();

    bool writeNeuronsToFile(const char *fname);
    bool readNeuronsFromFile(const char *fname);
};

