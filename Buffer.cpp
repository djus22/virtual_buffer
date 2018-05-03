#include "Buffer.h"

Buffer::Buffer()
{
	this->size = 0;
	this->emptyCounter = 0;
	this->emptySince = 0;
	this->emptyTime = 0;
}


int Buffer::getSize()
{
	return this->size;
}
void Buffer::setSize(int size)
{
	this->size = size;
}
void Buffer::incrSize(packet p)
{
	this->size += p.getLength();
	if(this->emptySince > 0)
		this->emptyTime += (p.getTime() - this->emptySince);
	this->emptySince = -1;
}

void Buffer::decrSize(packet p)
{
	if(this->size > p.getLength())
		this->size -= p.getLength();
	else
	{
		this->size = 0;
		this->emptySince = p.getTime();
	}
}


int Buffer::getEmptyCounter()
{
	return this->emptyCounter;
}
void Buffer::incrEmptyCounter()
{
	this->emptyCounter++;
}

bool Buffer::isEmpty()
{
	return (this->size) ? false : true;
}
double Buffer::getEmptyTime()
{
	return this->emptyTime;
}

Buffer::~Buffer()
{

}
