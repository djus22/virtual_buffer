#include "packet.h"



packet::packet()
{
	this->number = 0;
	this->time = 0;
	this->length = 0;
}
packet::packet(int number, double time, int length)
{
	this->number = number;
	this->time = time;
	this->length = length;
}

double packet::getTime()
{
	return this->time;
}

void packet::setTime(double t)
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

void packet::printPacket()
{
	cout << "packet number: " << this->number << "\n";
	cout << "packet time: " << setprecision(16) << this->time << " seconds" << "\n";
	cout << "packet length: " << this->length << " bytes" << "\n\n";

}
packet::~packet()
{

}


