#include "Buffer.h"


Buffer::Buffer(int size)
{
	this->size = size;
}
Buffer::Buffer()
{
	this->size = 0;
}

int Buffer::getSize()
{
	return this->size;
}
void Buffer::setSize(int size)
{
	this->size = size;
}
void Buffer::incrSize(int size)
{
	this->size += size;
}

int Buffer::getDeclCntr()
{
	return this->declineCounter;
}
void Buffer::incrDeclCntr(int decl)
{
	this->declineCounter += decl;
}
void Buffer::setDeclCntr(int decl)
{
	this->declineCounter = decl;
}

bool Buffer::isEmpty()
{
	return (this->size) ? false : true;
}

Buffer::~Buffer()
{

}
