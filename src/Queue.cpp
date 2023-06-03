#include "Queue.h"

Queue::Queue(int size) {
	_size = size;
	this->buffer = new Block[_size];
	putpos = 0;
	getpos = 0;
}

Queue::~Queue() {
	delete[] buffer;
	buffer = nullptr;
}

Block* Queue::_get(const char* consumername) {
	std::lock_guard<std::mutex> lock(mtx);

	Block* Block = &buffer[getpos];
	getpos = (getpos + 1) % _size;
	count--;

	return Block;
}

void Queue::_put(const char* producername, Block* Block)
{
	buffer[putpos] = *Block;
	putpos = (putpos + 1) % _size;
	count++;
}

int Queue::queueCount() const
{
	return count;
}
