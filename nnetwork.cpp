#include <nnetwork.h>
#define ISRAND true

neuralNetwork::neuralNetwork()
{
	//fuck off!
}

neuralNetwork::~neuralNetwork()
{
	free(neuronsInLayers);
	free(networkLayers);

	delete[] fileNames;
	delete[] networkInput;
	delete[] networkOutput;
}

void neuralNetwork::generateNames()
{
	string defname = "layer";
	fileNames = new string[layersCount];
	for(int i = 0; i < layersCount; i++)
	{
		fileNames[i] = defname + (const char)(i + '0') + ".bin";
	}

}

bool neuralNetwork::initNetwork()
{
	cout<<"Input the number of layers\n"; cin>>layersCount;
	cout<<"Input the size of input vector\n"; cin>>inputVectorSize;

	networkInput = new float[inputVectorSize];

	neuronsInLayers = (int *)calloc(layersCount, sizeof(int));
    generateNames(); //generate filenames
	for(int i = 0; i < layersCount; i++)
	{
		cout<<"Input the number of neurons in "<<i<<"'th layer";
		cin>>neuronsInLayers[i];
	}

	networkOutput = new float[neuronsInLayers[layersCount - 1]]; //Initialize the output vector

	networkLayers = (layer *)calloc(layersCount, sizeof(layer));

	networkLayers[0].initLayer(neuronsInLayers[0], inputVectorSize);
	networkLayers[0].constructNeurons(ISRAND, fileNames[0]);
	for(int i = 1; i < layersCount; i++)
	{
		networkLayers[i].initLayer(neuronsInLayers[i], neuronsInLayers[i - 1]);
		networkLayers[i].constructNeurons(ISRAND, fileNames[i]);
	}
}

void neuralNetwork::getInput(const char *fname, int k) //k - ordinary number of vector
{
    FILE *fp;
    if((fp = fopen(fname, "rb+")) == NULL)
    {
        fout<<"Error reading input vectors! Function neuralNetwork::getInput.";
        exit(1);
    }

    fseek(fp, (sizeof(float) * inputVectorSize * k), SEEK_SET);

    for(int i = 0; i < inputVectorSize; i++)
    {
        fread(&networkInput[i], sizeof(float), 1, fp);
    }

    fclose(fp);

}

void neuralNetwork::scaleInput(float vecMax, float vecMin, int startInterval, int endInterval)
{
    for(int i = 0; i < inputVectorSize; i++)
    {
        networkInput[i] = ((networkInput[i] - vecMin) * (endInterval - startInterval) / (vecMax - vecMin)) + endInterval;
    }
}

void neuralNetwork::reverseScaleOutput(float vecMax, float vecMin, int startInterval, int endInterval)
{
    for(int i = 0; i < (sizeof(networkOutput) / sizeof(float)); i++)
    {
        networkOutput[i] = vecMin + ((networkOutput[i] - startInterval) * (vecMax - vecMin) / (endInterval - startInterval));
    }
}

void neuralNetwork::processLayersData()
{
    //getInput and scaleInput are executed from main()
    //We have scaled input vector now
    for(int i = 0; i < networkLayers[0].inputsCount; i++)
    {
        networkLayers[0].inputs[i] = networkInput[i];
    }

    for(int i = 0; i < layersCount; i++)
    {
        networkLayers[i].computeOutput();
        for(int j = 0; j < networkLayers[i + 1].inputsCount; j++)
        {
            networkLayers[i + 1].inputs[j] = networkLayers[i].outputs[j];
        }

    }

    //reverseScaleOutput is done from main()

}
