#pragma once

#include "Block.h"

class Queue
{
private:

	// Twee indexen en de lengte bijhouden.
	// Redundant, maar lekker makkelijk!
	int getpos, putpos;
	int count;
	int _size;

	Block* buffer = nullptr;
public:
	Queue(int size);
	virtual ~Queue();
	Block* _get(const char* consumername);

	void _put(const char* producername, Block* Block);
};