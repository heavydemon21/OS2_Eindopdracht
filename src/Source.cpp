#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>

#include "Source.h"

#include <iostream>
#include <vector>
#include <thread>



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

    FILE* inputFile = fopen( inputFileName.c_str(), "rb");
    FILE* outputFile = fopen( outputFileName.c_str(), "wb");

    if (!inputFile) {
        std::cerr << "Failed to open input.pcm" << std::endl;
        return 1;
    }

    long inputF_size = fileSize(inputFile);
    int numBlocks = inputF_size / BLOCK_SIZE;

    Queue blockQueue(numBlocks);

    for (int i = 0; i < numBlocks; i++)
    {
        fseek(inputFile, i * numBlocks, SEEK_END);
        
        int16_t data[BLOCK_SIZE];
        fread(data, sizeof(int16_t), BLOCK_SIZE, inputFile);

        Block* newBlock = new Block(i);
        newBlock->setData(data);

        blockQueue._put("Producer", newBlock);
        if (newBlock->getIndex() ==0) {
            std::cout << data << std::endl;
        }
    }

    fclose(inputFile);

    // Init workers
    Worker** workers = new Worker*[numThreads];

    for (int i = 0; i < numThreads; i++)
    {
        workers[i] = new Worker;
        workers[i]->set_bass(bb0,bb1,bb2,ba1,ba2);
        workers[i]->set_treble(tb0,tb1,tb2,ta1,ta2);
    }
    int i = 0;
    std::vector<std::thread> workerThreads;

    while (blockQueue.queueCount() != 0)
    {
        if (!workers[i]->isDone()) {
            
            //Assign to worker found in prev step
            Block* data = workers[i]->new_block(blockQueue._get("consumer"));//pops block address

            fill_FILE(data, outputFile);

            std::thread workerThread(&Worker::work, workers[i]);
            workerThread.detach();

            workerThreads.push_back(std::move(workerThread));
        }
      

        //Store new blocks (if worker done) somewhere,
        //Mark that worker as free again.

        i = (i + 1) % numThreads;

    }
    //bool allThreadsDone = false;
    //while (!allThreadsDone) {
    //    for (int tel = 0; tel < numThreads; tel++) {
    //        if (!workers[i]->isDone()) {
    //            tel = 0;
    //        }
    //    }

    //    allThreadsDone = true;
    //}


    workerThreads.clear();
    delete[] workers;


    fclose(outputFile);
	return 0;
}