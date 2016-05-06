#include <iostream>
#include "nnetwork.h"
#include <conio.h>
int main()
{
    neuralNetwork network;
    network.initNetwork();
    FILE *fp=fopen("layer0input.bin","wb+");
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
    fp=fopen("layer0input.bin","rb+");
    network.getInput("layer0input.bin",0);
   // network.scaleInput(0.0,10.0,0.0,1.0);
    network.processLayersData();
    for(int i = 0; i < network.neuronsInLayers[network.layersCount - 1]; i++)
    {
         cout<<network.networkOutput[i]<<" ";
    }
    network.writeWeightsToFiles();
    fclose(fp);
	_getch();
    return 0;
}
