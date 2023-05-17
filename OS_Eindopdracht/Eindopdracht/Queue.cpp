#include "Queue.h"

Queue::Queue(int size) {
	_size = size;
	this->buffer = new Block[_size];

}

Queue::~Queue() {
	delete[] buffer;
	buffer = nullptr;
}

Block* Queue::_get(const char* consumername) {
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