#pragma once
#include "DJ_header.h"


class Buffer
{
public:
	Buffer();


	int getSize();
	void setSize(int size);
	void incrSize(packet p);
	void decrSize(packet p);

	double getEmptyTime();
	int getEmptyCounter();
	void incrEmptyCounter();


	bool isEmpty();


	~Buffer();
private:
	int size;
	int emptyCounter;

	double emptySince; // time when buffer was empty | if variable is -1 it means the size > 0
	double emptyTime; // all time when buffer was empty

};

