//
//  main.cpp
//  ate
//
//  Created by Koen van Brero on 12-05-14.
//  Copyright (c) 2014 Koen van Brero. All rights reserved.
//

#include <iostream>
#import <pthread.h>
#include <semaphore.h>

const int BUFLEN = 20;   // de lengte van de queue

#include <math.h>

void bassCoefficients(int intensity, double *b0, double *b1, double *b2, double *a1, double *a2)
{
    double frequency = 330;
    double qFactor = 0.5;
    double gain = intensity;
    double sampleRate = 44100;
    
    double pi=4.0*atan(1);
    double a=pow(10.0, gain/40);
    double w0=2*pi*frequency/sampleRate;
    double alpha=sin(w0)/(2.0*qFactor);
    double a0=(a+1)+(a-1)*cos(w0)+2.0*sqrt(a)*alpha;
    
    *a1=-(-2.0*((a-1)+(a+1)*cos(w0)))/a0;
    *a2=-((a+1)+(a-1)*cos(w0)-2.0*sqrt(a)*alpha)/a0;
    
    *b0=(a*((a+1)-(a-1)*cos(w0)+2.0*sqrt(a)*alpha))/a0;
    *b1=(2*a*((a-1)-(a+1)*cos(w0)))/a0;
    *b2=(a*((a+1)-(a-1)*cos(w0)-2.0*sqrt(a)*alpha))/a0;
}

void trebleCoefficients(int intensity, double *b0, double *b1, double *b2, double *a1, double *a2)
{
    double frequency = 3300;
    double qFactor = 0.5;
    double gain = intensity;
    double sampleRate = 44100;
    
    double pi=4.0*atan(1);
    double a=pow(10.0, gain/40);
    double w0=2*pi*frequency/sampleRate;
    double alpha=sin(w0)/(2.0*qFactor);
    double a0=(a+1)-(a-1)*cos(w0)+2.0*sqrt(a)*alpha;
    
    *a1=-(2.0*((a-1)-(a+1)*cos(w0)))/a0;
    *a2=-((a+1)-(a-1)*cos(w0)-2.0*sqrt(a)*alpha)/a0;
    
    *b0=(a*((a+1)+(a-1)*cos(w0)+2.0*sqrt(a)*alpha))/a0;
    *b1=(-2.0*a*((a-1)+(a+1)*cos(w0)))/a0;
    *b2=(a*((a+1)+(a-1)*cos(w0)-2.0*sqrt(a)*alpha))/a0;
}

class Block
{
private:
    unsigned char data[2048];
    int phase; // 0 = created, 1 = filled,  2 = bassed, 3 = bassed+trebled, 4 = written
    int index;
    
public:
    Block(int newIndex)
    {
        index=newIndex;
        phase=0;
    }
    
    void setData(char* newData)
    {
        for(int i=0;i<2048;i++)
        {
            data[i] = newData[i];
        }
    }
    
    void setPhase(int newPhase)
    {
        phase=newPhase;
    }
    
    int getPhase()
    {
        return phase;
    }
    
    int getIndex()
    {
        return index;
    }
};


// De ronde wachtrij
// Die moet je dus thread-safe maken!
class Queue
{
private:
	Block *buffer[BUFLEN];
    
	// Twee indexen en de lengte bijhouden.
	// Redundant, maar lekker makkelijk!
	int getpos, putpos;
	int count;
        
public:
	Queue()
	{        
		getpos = 0;
		putpos = 0;
		count = 0;
		for(int i = 0; i < BUFLEN; i++) buffer[i] = 0;
	}
    
	Block *_get(const char *consumername)
	{
			
		Block *Block = buffer[getpos];
		getpos = (getpos + 1) % BUFLEN;
		count--;

		return Block;
	}
    
	void _put(const char *producername, Block *Block)
	{
		buffer[putpos] = Block;
		putpos = (putpos + 1) % BUFLEN;
		count++;
	}
};

// Fijne globale variabele: de wachtrij
Queue q;

int main(int argc, const char * argv[])
{
    
    double ba1,ba2,bb0,bb1,bb2;
    double ta1,ta2,tb0,tb1,tb2;
    
    bassCoefficients(3, &bb0,&bb1,&bb2,&ba1,&ba2);
    trebleCoefficients(-4, &tb0,&tb1,&tb2,&ta1,&ta2);
    
    if (argc != 5)
    {
        std::cout << "Error: Incorrect number of parameters !" << std::endl;
        std::cout << "Usage: ate -p:<number of threads> -b:<bass intensity> -t:<trebble intensity> <input file> <output file>" << std::endl;
        exit(1);
    }

    FILE *inputFile;
    FILE *outputFile;
    
    inputFile = fopen("input.wav","rb");
    outputFile = fopen("output.wav", "wb");
    
  //  int threads = 8;

    
  
    
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}

void Worker()
{
    
    Block* myBlock=q._get("?");
    
    if(myBlock->getPhase() == 0)
    {
        std::cout << "Worker(): ERROR block with no data" << std::endl;
    }
    else if(myBlock->getPhase() == 1)
    {
        //  performBassEQ(myBlock);
        std::cout << "Worker(): Processed bass on block: " << myBlock->getIndex() << std::endl;
        myBlock->setPhase(2);
        q._put("?", myBlock);
    }
    else if(myBlock->getPhase() == 2)
    {
        //  performTrebleEQ(myBlock);
        std::cout << "Worker(): Processed treble on block: " << myBlock->getIndex() << std::endl;
        myBlock->setPhase(3);
        q._put("?", myBlock);
    }
    else if(myBlock->getPhase() == 3)
    {
        //  performBassEQ(myBlock);
        std::cout << "Worker(): Written out processed data in block: " << myBlock->getIndex() << std::endl;
        myBlock->setPhase(4);
    }
    
}
