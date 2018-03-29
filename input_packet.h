/*
 * input_packet.h
 *
 *  Created on: Mar 29, 2018
 *      Author: dawid
 */
#pragma once

#include "packet.h"
#include <iostream>
#include <iomanip>

using namespace std;

class input_packet: public packet {
public:
	input_packet();
	input_packet(int num, double time, int len);

	virtual double getTime();
	virtual void setTime(double t);
	virtual int getLength();
	virtual void setLength(int l);
	virtual int getNumber();
	virtual void setNumber(int n);

	virtual void printPacket();

	virtual ~input_packet();

private:
	int number;
	double time;
	int length;

};
