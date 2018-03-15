#pragma once
class Buffer
{
public:
	Buffer();
	Buffer(int s);


	int getSize();
	void setSize(int size);
	void incrSize(int size);

	int getDeclCntr();
	void setDeclCntr(int decl);
	void incrDeclCntr(int decl);

	bool isEmpty();


	~Buffer();
private:
	int size;
	int emptyCounter;
	int declineCounter;

};

