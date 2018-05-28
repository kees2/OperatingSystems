// OSEindopdracht.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>

using namespace std;
#include <io.h>

int main(int argc, char** argv)
{
	cout << "Das program hat angefangen" << endl;

	for (int i = 0; i < argc; i++) {
		cout << argv[i] << endl;
		if (argv[i][0] == 'k') {
			cout << "k is ingegeven jij engert" << endl;
			cout << "Tweede argument is " << argv[i][1] << endl;
		}
	}

	cin.get();

	
	FILE * infile = fopen("you_and_i.pcm", "rb");     // Open wave file in read  

	fseek(infile, 0, SEEK_END);
	int fileSize = ftell(infile);
	fseek(infile, 0, SEEK_SET);

	signed short * buf = (signed short *)malloc(fileSize);
	signed short * f = (signed short *)malloc(fileSize * sizeof(signed short));

	//read buffer
	fread(buf, sizeof(signed short), fileSize, infile);




    return 0;
}

