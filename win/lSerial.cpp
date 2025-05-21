#include "pch.h"
#include "win/lSerial.h"

lSerial::lSerial()
{
	Initialization();
}

lSerial::~lSerial()
{
	Close();
}

void lSerial::Initialization()
{
	lThread::Initialization();
	shdr = NULL;
	memset(&SerialParmeter, 0, sizeof(DCB));
	memset(&timer,0,sizeof(COMMTIMEOUTS));
}

void lSerial::Close()
{
	lThread::Close();
	CloseHandle(shdr);
	memset(&SerialParmeter, 0, sizeof(DCB));
	memset(&timer, 0, sizeof(COMMTIMEOUTS));
}

BOOL lSerial::run()
{

	return lThread::run();
}

void lSerial::Open(const char* name, int baudRate,int dt){
	CString strNmae(name);
	shdr = CreateFile(strNmae,
		GENERIC_READ | GENERIC_WRITE, 
		0, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if (!GetCommState(shdr, &SerialParmeter)) {
		CloseHandle(shdr);
		return;
	}
	SerialParmeter.DCBlength = sizeof(SerialParmeter);
	switch (baudRate) {
	case 9600:
		SerialParmeter.BaudRate = CBR_9600;
		break;
	case 115200:
		SerialParmeter.BaudRate = CBR_115200;
		break;
	}
	SerialParmeter.ByteSize = 8;
	SerialParmeter.StopBits = ONESTOPBIT;
	SerialParmeter.Parity = NOPARITY;
	if (!SetCommState(shdr, &SerialParmeter)) {
		CloseHandle(shdr);
		return;
	}
	start(dt);
}
