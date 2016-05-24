#include <iostream>
#include <vector>
#include "EasyBMP.h"

using namespace std;

int sum_range(BMP bkg, int start_x, int finish_x, int start_y, int finish_y, int black_coeff)
{
	RGBApixel tmp;

	int blacks = 0;
	int whites = 0;

	cout<<"\n----------\n";
		for(int j = start_y; j < finish_y; j++) {
	for(int i = start_x; i < finish_x; i++)	{
			tmp = bkg.GetPixel(i,j);

			if(tmp.Red == 255 && tmp.Green == 255 && tmp.Blue == 255) cout<<"0";
			else cout<<"1";
			
		}
		cout<<"\n";
	}
	cout<<"----------\n";
	for(int i = start_x; i < finish_x; i++)	{
		for(int j = start_y; j < finish_y; j++) {
			tmp = bkg.GetPixel(i,j);

			if(tmp.Red == 255 && tmp.Green == 255 && tmp.Blue == 255) whites+=1;
			else blacks+=1;
			
		}
	}

	blacks *= black_coeff;

	cout<<"\nBL: "<<blacks<<"\n";
	cout<<"WH: "<<whites<<"\n";

	if(blacks >= whites) return 1;
	else return -1;
}

int main(int argc, char *argv[])
{
	const char *fname = "input.bin";
	FILE *fp;
	int sx,sy;

	int width, height;

	vector<float> out_vec;
	int m = 0; /* Counter for the vector */

	sx = 3, sy = 4;

	out_vec.resize(sx * sy);

	BMP bkg;
	bkg.ReadFromFile(argv[1]);
	width = bkg.TellWidth();
	height = bkg.TellHeight();

	cout<<"WIDTH: "<<width<<"\n";
	cout<<"HEIGHT: "<<height<<"\n";

	RGBApixel tmp;

	int blacks = 0, whites = 0;
	int black_coeff = 0;

	for(int i = 0; i < bkg.TellHeight(); i++) {
		for(int j = 0; j < bkg.TellWidth(); j++) {
			tmp = bkg.GetPixel(j,i);

			if(tmp.Red == 255 && tmp.Green == 255 && tmp.Blue == 255) cout<<"0";
			else cout<<"1";

		}
		cout<<"\n";
	}

	for(int i = 0; i < bkg.TellHeight(); i++) {
		for(int j = 0; j < bkg.TellWidth(); j++) {
			tmp = bkg.GetPixel(j,i);

			if(tmp.Red == 255 && tmp.Green == 255 && tmp.Blue == 255) whites+=1;
			else blacks+=1;

		}
	}

	cout<<"BLACK: "<<blacks<<"\n";
	cout<<"WHITE: "<<whites<<"\n";

	/* Compute coefficient */

	black_coeff = (int)(whites/blacks);

	cout<<"BLACK_COEFF IS: "<<black_coeff<<"\n";


	for(int i = 0, fi = (width/sx); i < width, fi < width; i += (width/sx), fi += (width/sx)) {
		for(int j = 0, fj = (height/sy); j < height, fj < height; j += (height/sy), fj += (height/sy)) {
/*				int st_x, fin_x, st_y, fin_y;

			st_x = (int)(width/sx) * i;
			fin_x = (int)(width/(sx - i));

			st_y = (int)(height/sy) * j;
			fin_y = (int)(height/(sy - j));
*/

			cout<<"Component "<<m<<": "<<"\n";
			cout<<"st_x: "<<i<<"\n";
			cout<<"fin_x: "<<fi<<"\n";
			cout<<"st_y: "<<j<<"\n";
			cout<<"fin_y: "<<fj<<"\n";

			out_vec[m] = sum_range(bkg, i, fi, j, fj, black_coeff);
			cout<<"Component "<<m<<" is: "<<out_vec[m]<<"\n";

			m += 1;


		}
	}


	for(int i = 0; i < bkg.TellHeight(); i++) {
		for(int j = 0; j < bkg.TellWidth(); j++) {
			tmp = bkg.GetPixel(j,i);

			if(tmp.Red == 255 && tmp.Green == 255 && tmp.Blue == 255) cout<<" ";
			else cout<<"0";
		}
		cout<<"\n";
	}

	fp = fopen(fname, "ab+");
	if(fp == NULL) cout<<"FUUUUUKKKKKKK";
	for(int i = 0; i < out_vec.size(); i++) {
		fwrite(&out_vec[i], sizeof(float), 1, fp);

	}

	fclose(fp);
/*	
	for(int i = 0; i < bkg.TellHeight(); i++) {
		for(int j = 0; j < bkg.TellWidth(); j++) {
			tmp = bkg.GetPixel(j,i);

			cout<<"(" << (int)tmp.Red << ","
					  << (int)tmp.Green << ","
					  << (int)tmp.Blue <<","
					  << (int)tmp.Alpha <<")\n";
					  

			if(tmp.Red == 255 && tmp.Green == 255 && tmp.Blue == 255) cout<<" ";
			else cout<<"0";

		}
		cout<<"\n";
	}
	*/

	return 0;

}
