#pragma once
#include <iostream>
#include <iomanip>
using namespace std;

class packet
{
public:
	packet();
	packet(int num, double time, int len);
	virtual double getTime();
	virtual void setTime(double t);
	virtual int getLength();
	virtual void setLength(int l);
	virtual int getNumber();
	virtual void setNumber(int n);
	virtual void printPacket();
	virtual ~packet();
private:
	int number;
	double time;
	int length;

};
