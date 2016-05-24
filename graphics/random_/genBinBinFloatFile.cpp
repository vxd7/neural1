#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stack>

using namespace std;

int main(int argc, char *argv[])
{
	FILE *fp;

	if(argc == 1) {
		cout<<"prg.bin inFile outFile\n";
		return 1;
	}

	string instreamName(argv[1]);
	const char *fname = argv[2];

	fp = fopen(fname, "ab+");
	ifstream fin(instreamName.c_str());

	int tmp;
	int len = 0;
	while(fin>>tmp) {
		stack<int> bindigs;

		while(tmp != 0) {
			bindigs.push(tmp%2);
			tmp /= 2;
			len++;
		}

		for(int i = len; i < 4; i++)
			bindigs.push(0);

		float buf;
		for(int i = 0; i < 4; i++) {
			buf = (float)bindigs.top();
			bindigs.pop();
			cout<<buf<<" ";
			fwrite(&buf, sizeof(float), 1, fp);
		}
		
		len = 0;
		cout<<"\n";
	}


	fclose(fp);
	return 0;
}
