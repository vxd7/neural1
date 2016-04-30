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

	bool initNetwork();
	void generateNames();

    void getInput(); //from keyboard
	void getInput(const char *fname); //from file
	void scaleInput(float vecMax, float vecMin, int startInterval, int endInterval); //Preprocess input vector in order to fit inc
	void reverseScaleOutput(float vecMax, float vecMin, int startInterval, int endInterval);

	void processLayersData();

};
