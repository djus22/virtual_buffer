#pragma once
class packet
{
public:
	packet();
	int getTime();
	void setTime(int t);
	int getLength();
	void setLength(int l);
	int getNumber();
	void setNumber(int n);
private:
	int number;
	int time;
	int length;
};