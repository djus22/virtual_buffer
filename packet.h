#pragma once
#include <iostream>
#include <iomanip>
using namespace std;




class packet
{
public:
	packet();
	packet(int num, double time, int len);
	double getTime();
	void setTime(double t);
	int getLength();
	void setLength(int l);
	int getNumber();
	void setNumber(int n);
	char* printPacket();
	virtual ~packet();
private:
	int number;
	double time;
	int length;

};
