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
	static bool sortByTime(packet &p1 , packet &p2);
	static bool sortByNumber(packet &p1 , packet &p2);
	virtual ~packet();
private:
	long long int number;
	double time;
	int length;

};
