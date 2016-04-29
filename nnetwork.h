#include "layer.h"

class neuralNetwork{
public:
    layer *networkLayers;

    int layersCount;
    int *neuronsInLayers;

	int inputVectorSize;

    float *networkInput;
    float *networkOutput;

	string *fileNames;

    neuralNetwork();
    ~neuralNetwork();

	bool constructLayers();
};
