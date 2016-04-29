#include <nnetwork.h>
#define ISRAND true

neuralNetwork()
{
	//fuck off!
}

~neuralNetwork()
{
	delete[] neuronsInLayers;
	delete[] networkLayers;

	delete[] fileNames;
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

bool neuralNetwork::constructLayers()
{
	cout<<"Input the number of layers\n"; cin>>layersCount;
	cout<<"Input the size of input vector\n"; cin>>inputVector;
	
	neuronsInLayers = new int[layersCount];
	for(int i = 0; i < layersCount; i++)
	{
		cout<<"Input the number of neurons in "<<i<<"'th layer";
		cin>>neuronsInLayers[i];
	}

	networkLayers = new layer[layersCount];

	networkLayers[0].initLayer(neuronsInLayers[0], inputsVectorSize);
	networkLayers[0].constructNeurons(ISRAND, );
	for(int i = 1; i < layersCount; i++)
	{
		networkLayers[i].initLayer(neuronsInLayers[i], neuronsInLayers[i - 1]);
	}

	generateNames(); //generate filenames

	for(int i = 0; i < layersCount; i++)
	{
		networkLayers
	}
}
