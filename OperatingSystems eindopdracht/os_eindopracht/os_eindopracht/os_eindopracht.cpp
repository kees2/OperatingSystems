#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include "File.h"

#define default_threads 8;
#define default_bass_intensity 1;
#define default_treble_intensity 1;
#define default_input "you_and_i.pcm";
#define default_output "OutputFileV2.pcm";

int main(int argc, char** argv)
{
	int threads = default_threads;
	int bassIntensity = default_bass_intensity;
	int trebleIntensity = default_treble_intensity;
	char * inputFile = (char*)default_input;
	char * outputFile = (char*)default_output;
	std::vector<std::thread> threadObj;
	int bassThreads, trebleThreads, writeThreads;
	std::string str;
	int tempint;

	if (argc != 5)
	{
		if (argc == 11)
		{
			for (int i = 1; i < argc; i += 2) {
				str = argv[i];
				if (str == "p") {
					tempint = std::stoi(argv[i + 1]);
					if ((tempint >= 1) && (tempint <= 8))
						threads = tempint;
					else
					{
						std::cout << "aantal threads moet 1 t/m 8 zijn" << std::endl;
						return 0;
					}
				}
				else if (str == "b") {
					tempint = std::stoi(argv[i + 1]);
					if ((tempint >= -6) && (tempint <= 6))
					{
						bassIntensity = tempint;
						if (tempint == 0)
							bassIntensity = default_bass_intensity;
					}
					else
					{
						std::cout << "bassintensity moet -6 t/m 6 zijn" << std::endl;
						return 0;
					}
				}
				else if (str == "t") {
					tempint = std::stoi(argv[i + 1]);
					if ((tempint >= -6) && (tempint <= 6))
					{
						trebleIntensity = tempint;
						if (tempint == 0)
							trebleIntensity = default_treble_intensity;
					}
					else
					{
						std::cout << "trebleintensity moet -6 t/m 6 zijn" << std::endl;
						return 0;
					}
				}
				else if (str == "inputFile") {
					str = argv[i + 1];
					if (str.substr(str.find_last_of(".") + 1, str.length() - str.find_last_of(".")) == "pcm")
						inputFile = argv[i + 1];
					else
					{
						std::cout << "inputfile moet .pcm file zijn" << std::endl;
						return 0;
					}
				}
				else if (str == "outputFile") {
					str = argv[i + 1];
					if (str.substr(str.find_last_of(".") + 1, str.length() - str.find_last_of(".")) == "pcm")
						outputFile = argv[i + 1];
					else
					{
						std::cout << "outputfile moet .pcm file zijn" << std::endl;
						return 0;
					}
				}
			}
		}
		else
		{
			std::cout << "Foute aantal parameters" << std::endl;
			return 0;
		}
	}
/*
	if (argc != 5)
	{
		if (argc == 6)
		{
			tempint = std::stoi(argv[1]);
			if ((tempint >= 1) && (tempint <= 8))
				threads = tempint;
			else
			{
				std::cout << "aantal threads moet 1 t/m 8 zijn" << std::endl;
				return 0;
			}
			tempint = std::stoi(argv[2]);
			if ((tempint >= -6) && (tempint <= 6))
			{
				bassIntensity = tempint;
				if (tempint == 0)
					bassIntensity = default_bass_intensity;
			}
			else
			{
				std::cout << "bassintensity moet -6 t/m 6 zijn" << std::endl;
				return 0;
			}
			tempint = std::stoi(argv[3]);
			if ((tempint >= -6) && (tempint <= 6))
			{
				trebleIntensity = tempint;
				if (tempint == 0)
					trebleIntensity = default_treble_intensity;
			}
			else
			{
				std::cout << "trebleintensity moet -6 t/m 6 zijn" << std::endl;
				return 0;
			}
			str = argv[4];
			if (str.substr(str.find_last_of(".") + 1, str.length() - str.find_last_of(".")) == "pcm")
				inputFile = argv[4];
			else
			{
				std::cout << "inputfile moet .pcm file zijn" << std::endl;
				return 0;
			}
			str = argv[5];
			if (str.substr(str.find_last_of(".") + 1, str.length() - str.find_last_of(".")) == "pcm")
				outputFile = argv[5];
			else
			{
				std::cout << "outputfile moet .pcm file zijn" << std::endl;
				return 0;
			}
		}
		else
		{
			std::cout << "Foute aantal parameters" << std::endl;
			return 0;
		}
	}
*/
	File file(inputFile, outputFile, bassIntensity, trebleIntensity);
	threadObj.push_back(std::thread(&File::read, &file));
	bassThreads = trebleThreads = writeThreads = (threads - 1) / 3;
	if ((threads - 1) % 3)
		bassThreads++;
	if (((threads-1) % 3) == 2)
		trebleThreads++;
	trebleThreads += bassThreads;
	writeThreads += trebleThreads;
	for (int i = 0; i < (threads - 1); i++)
	{
		if (i < bassThreads)
			threadObj.push_back(std::thread(&File::bassMod, &file));
		else if (i < trebleThreads)
			threadObj.push_back(std::thread(&File::trebleMod, &file));
		else if (i < writeThreads)
			threadObj.push_back(std::thread(&File::writeBuf, &file));
	}

	for (auto& th : threadObj)
		th.join();

	if (threads == 1)
		file.bassMod();
	if (threads <= 2)
		file.trebleMod();
	if (threads <= 3)
		file.writeBuf();
	file.writeFile();

	std::cin.get();
    return 0;
}
