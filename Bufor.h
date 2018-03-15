#pragma once
class Bufor
{
public:
	Bufor(int s);
	int getSize();
	void setSize(int s);

	~Bufor();
private:
	int size;
	int emptyCounter;
	int declineCounter;

};

