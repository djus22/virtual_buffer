#include "packet.h"

packet::packet()
{
}

int packet::getTime()
{
	return 0;
}

void packet::setTime(int t)
{
	this->time = t;
}

int packet::getLength()
{
	return this->length;
}

void packet::setLength(int l)
{
	this->length = l;
}

int packet::getNumber()
{
	return this->number;
}

void packet::setNumber(int n)
{
	this->number = n;
}
