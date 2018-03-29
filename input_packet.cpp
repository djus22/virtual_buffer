/*
 * input_packet.cpp
 *
 *  Created on: Mar 29, 2018
 *      Author: dawid
 */

#include "input_packet.h"



input_packet::input_packet()
{
	this->number = 0;
	this->time = 0;
	this->length = 0;
}
input_packet::input_packet(int number, double time, int length)
{
	this->number = number;
	this->time = time;
	this->length = length;
}
double input_packet::getTime()
{
	return this->time;
}
void input_packet::setTime(double t)
{
	this->time = t;
}
int input_packet::getLength()
{
	return this->length;
}
void input_packet::setLength(int l)
{
	this->length = l;
}
int input_packet::getNumber()
{
	return this->number;
}
void input_packet::setNumber(int n)
{
	this->number = n;
}

void input_packet::printPacket()
{
	cout << "packet number: " << this->number << "\n";
	cout << "packet time: " << setprecision(16) << this->time << " seconds" << "\n";
	cout << "packet length: " << this->length << " bytes" << "\n\n";

}
input_packet::~input_packet()
{

}

