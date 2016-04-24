#include "layer.h"

class neuralNetwork{
public:
    layer *networkLayers;

    int layersCount;
    int *neuronsInLayers;

    float *networkInput;
    float *networkOutput;




    neuralNetwork();
    ~neuralNetwork();


};
