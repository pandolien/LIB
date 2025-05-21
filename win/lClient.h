#ifndef __LCLIENT_H__
#define __LCLIENT_H__
#include <winsock2.h>
#include "win/lThread.h"
#pragma comment(lib, "ws2_32.lib")
class lClient :public lThread {
public:
	lClient();
	~lClient();
public:
	SOCKET sock;
	int Error;
public:
	void Initialization() override;
	void Close() override;
	void Connect(const char*, int,int);
public:
	void Recv(char*, unsigned int);
	void Send(char*, unsigned int);
public:
	virtual BOOL run() override;
};
#endif