#include "Source.h"

#include <iostream>
#include <string>
#include <fstream>


#include "Queue.h"
const int16_t 2048;

int main(int argc, const char* argv[]) {

    // starting values 
	int numThreads = 1;
	int bassIntensity = 0;
	int trebleIntensity = 0;

    double ba1, ba2, bb0, bb1, bb2;
    double ta1, ta2, tb0, tb1, tb2;

	std::string inputFileName;
	std::string outputFileName;

	if (argc != 6) {
		std::cout << "Error: Incorrect number of parameters !" << std::endl;
		std::cout << "Usage: ate -p:<number of threads> -b:<bass intensity> -t<treble intensity> <input file> <output file>" << std::endl;
		exit(1);
	}

    // retrieve command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg.find("-p:") == 0) {
            numThreads = std::stoi(arg.substr(3));
        }
        else if (arg.find("-b:") == 0) {
            bassIntensity = std::stoi(arg.substr(3));
        }
        else if (arg.find("-t:") == 0) {
            trebleIntensity = std::stoi(arg.substr(3));
        }
        else if (inputFileName.empty()) {
            inputFileName = arg;
        }
        else if (outputFileName.empty()) {
            outputFileName = arg;
        }
    }


    // Init
    bassCoefficients(bassIntensity, &bb0, &bb1, &bb2, &ba1, &ba2);
    trebleCoefficients(trebleIntensity, &tb0, &tb1, &tb2, &ta1, &ta2);

    FILE* inputFile = fopen(inputFileName.c_str(), "rb");
    FILE* outputFile = fopen(outputFileName.c_str(), "wb");

    int16_t inputF_size = fileSize(inputFile);



    Queue blockQueue(inputF_size);

	return 0;
}