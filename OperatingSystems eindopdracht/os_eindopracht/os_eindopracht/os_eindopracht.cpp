#include <iostream>
#include <thread>
#include "File.h"

int main(int argc, char** argv)
{
	/*
	int num_t = 8;
	int bassIntensity;
	int trebleIntensity;
	char * inputFile;
	char * outputFile;
	if (argc == 6)
	{
	if ((argv[1] >= 1) && (arv[1] <= 8))
	threads = argv[1];
	else
	std::cout << "aantal threads moet 1 t/m 8 zijn" << std::endl;
	if ((argv[2] >= -6) && (arv[2] <= 6))
	bassIntensity = argv[2];
	else
	std::cout << "bass intensity moet -6 t/m 6 zijn" << std::endl;
	if ((argv[3] >= -6) && (arv[3] <= 6))
	trebleIntensity = argv[3];
	else
	std::cout << "bass intensity moet -6 t/m 6 zijn" << std::endl;
	if (argv[4] == "you_and_i.pcm")
	inputFile = argv[4];
	else
	std::cout << "foute input file" << std::endl;
	if (argv[5] == "OutFile.pcm")
	inputFile = argv[5];
	else
	std::cout << "foute output file" << std::endl;
	}
	else
	std::cout << "Foute parameters" << std::endl;

	File file(inputFile, outputFile, bassIntensity, trebleIntensity);
	*/

	File file("you_and_i.pcm", "OutputFileV2.pcm", 3, 3);
	
	std::thread readThread(&File::read, &file);
	
	std::thread bassThread0(&File::bassMod, &file);
	std::thread bassThread1(&File::bassMod, &file);

	std::thread trebleThread0(&File::trebleMod, &file);
	std::thread trebleThread1(&File::trebleMod, &file);
	
	std::thread writeThread0(&File::writeBuf, &file);
	std::thread writeThread1(&File::writeBuf, &file);
	
	readThread.join();
	
//	if (bassThread0.joinable()) {
		bassThread0.join();
//	}
//	if (bassThread1.joinable()) {
		bassThread1.join();
//	}
	
//	if (trebleThread0.joinable()) {
		trebleThread0.join();
//	}
//	if (trebleThread1.joinable()) {
		trebleThread1.join();
//	}
//	if (writeThread0.joinable()) {
		writeThread0.join();
//	}
//	if (writeThread1.joinable()) {
		writeThread1.join();
//	}

	file.writeFile();


    return 0;
}
