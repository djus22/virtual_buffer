#include "Buffer.h"

Buffer::Buffer()
{
	this->size = 0;
	this->emptyCounter = 0;
	this->emptySince = -999999999;
	this->emptyTime = 0;
	this->downloaded = 0;
	this->uploaded = 0;
}


int Buffer::getSize()
{
	return this->size;
}
void Buffer::setSize(int size)
{
	this->size = size;
}
void Buffer::incrSize(packet *p)
{
	this->size += p->getLength();
	this->uploaded += p->getLength();
	if(this->emptySince != -999999999)
	{
		this->emptyTime += (p->getTime() - this->emptySince);
		this->emptySince = -999999999;
	}
}

void Buffer::setEmptySince(packet *p)
{
	this->emptySince = p->getTime();
}

double Buffer::getEmptySince()
{
	return this->emptySince;
}

void Buffer::decrSize(packet *p)
{
	if(this->size > p->getLength())
	{
		this->size -= p->getLength();
		this->downloaded += p->getLength();
	}
	else
	{
		this->downloaded += this->size;
		this->size = 0;
		if(this->emptySince == -999999999)
			this->emptySince = p->getTime();
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
void Buffer::incrEmptyTime(packet *p)
{
	this->emptyTime += (p->getTime() - this->emptySince);
	this->emptySince = p->getTime();
}

int Buffer::getUploaded()
{
	return this->uploaded;
}
int Buffer::getDownloaded()
{
	return this->downloaded;
}


Buffer::~Buffer()
{

}
