#pragma once

#include "Block.h"

#include <mutex>


class Queue
{
private:

	// Twee indexen en de lengte bijhouden.
	// Redundant, maar lekker makkelijk!
	int getpos, putpos;
	int count;
	int _size;

	Block* buffer = nullptr;

	std::mutex mtx; 

public:
	Queue(int size);
	virtual ~Queue();
	Block* _get(const char* consumername);

	void _put(const char* producername, Block* Block);

	int queueCount() const;
};