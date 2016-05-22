#include <iostream>
#include "nnetwork.h"
#include <conio.h>
int main()
{
    neuralNetwork network;
    network.initNetwork();
    /*FILE *fp_in=fopen("SamplesInputsFile.bin","wb+");
	FILE *fp_out=fopen("SamplesOutputsFile.bin","wb+");
    float temp,tempout;
    int kol;
    cout<<"enter kol\n";
    cin>>kol;
   for(int i=0;i<kol;i++)
    {
       // temp=i*i;
		cout<<"Enter number\n";
		cin>>temp;
		tempout=temp*temp;
        fwrite(&temp,sizeof(float),1,fp_in);
		fwrite(&tempout,sizeof(float),1,fp_out);
		
    }
    fclose(fp_in);
	fclose(fp_out);*/
    //fp=fopen("layer0input.bin","rb+");
	network.learn("SamplesOutputsFile.bin", "SamplesInputsFile.bin", 0.05);
    network.getInput("SamplesInputsFile.bin",3);
    network.scaleInput("SamplesInputsFile.bin", 0.0, 1.0);
	network.processLayersData();
	network.reverseScaleOutput("SamplesOutputsFile.bin", 0.0, 1.0);
	for(int i = 0; i < network.neuronsInLayers[network.layersCount - 1]; i++)
    {
         cout<<network.networkOutput[i]<<" ";
    }
   
   
   // network.writeWeightsToFiles();
   // fclose(fp);
	_getch();

	return 0;
}
