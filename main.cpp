#include <iostream>
#include "nnetwork.h"
int main()
{
    neuralNetwork network;
    network.initNetwork();
  /*  FILE *fp=fopen("layer0input.bin","wb+");
    float temp;
    int kol;
    cout<<"enter kol\n";
    cin>>kol;
    for(int i=0;i<kol;i++)
    {
        cout<<"Enter number: ";
        cin>>temp;
        fwrite(&temp,sizeof(float),1,fp);
    }
    fclose(fp);
    fp=fopen("layer0input.bin","rb+");*/
	/*
    network.getInput("SamplesInputsFile.bin",0);
    network.scaleInput("SamplesInputsFile.bin", 0.0, 1.0);
	network.processLayersData();
	network.reverseScaleOutput("SamplesOutputsFile.bin", 0.0, 1.0); 
	*/
    network.learn("SamplesOutputsFile.bin", "SamplesInputsFile.bin", 0.1);
   /* for(int i = 0; i < network.neuronsInLayers[network.layersCount - 1]; i++)
    {
         cout<<network.networkOutput[i]<<" ";
    }*/
   // network.writeWeightsToFiles();
   // fclose(fp);

	return 0;
}
