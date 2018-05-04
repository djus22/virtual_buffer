#pragma once
#include "DJ_header.h"


class Buffer
{
public:
	Buffer();


	int getSize();
	void setSize(int size);
	void incrSize(packet *p);
	void decrSize(packet *p);

	double getEmptyTime();
	void incrEmptyTime(packet *p);
	int getEmptyCounter();
	void incrEmptyCounter();
	void setEmptySince(packet *p);
	double getEmptySince();
	int getUploaded();
	int getDownloaded();

	bool isEmpty();


	~Buffer();
private:
	int size;							// actual buffer size
	int uploaded;						// all data uploaded to buffer
	int downloaded;						// all data downloaded from buffer
	int emptyCounter;					// number of unsuccessful attempts of getting data from buffer

	double emptySince; 					// time when buffer was empty | if variable is -999999999 it means the size > 0
	double emptyTime; 					// all time when buffer was empty

};

