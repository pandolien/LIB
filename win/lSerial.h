#ifndef __LSERIAL_H__
#define __LSERIAL_H__
#include "lThread.h"
class lSerial :public lThread{
public:
	lSerial();
	~lSerial();
public:
	void Initialization() override;
	void Close() override;
	BOOL run()override;
public:
	HANDLE shdr;
	DCB SerialParmeter;
	COMMTIMEOUTS timer;
public:
	void Open(const char*, int,int);
};
#endif