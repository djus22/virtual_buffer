#pragma once
class packet
{
public:
	packet();
	virtual int getTime();
	virtual void setTime(int t);
	virtual int getLength();
	virtual void setLength(int l);
	virtual int getNumber();
	virtual void setNumber(int n);
private:
	int number;
	int time;
	int length;
};
