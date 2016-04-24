#include "neuron.h"

class layer{
public:
    neuron *neurons;
    int numberOfNeurons; //in current layer
    int inputsCount; //number of neurons in previous layer, inputs in current one

    float *inputs, *outputs;

    void initLayer(int neuronsC, int inputC);
    layer();
    ~layer();

    float computeBeta();

    void constructNeurons();

    float *computeOutput();

    bool writeNeuronsToFile(const char *fname);
};

