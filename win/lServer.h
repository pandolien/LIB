#ifndef __LSERVER_H__
#define __LSERVER_H__
#include "win/lClient.h"
#include "lPTRList.h"
class lServer :public lThread {
public:
	lServer();
	~lServer();
public:
	virtual void Initialization() override;
	virtual void Close() override;
public:
	WSADATA wsaData;
	SOCKET sock;
	lPTRList ltClient;
	fd_set sltAccept;
	unsigned short Error;
public:
	void ServerTCPIPOpen(const char*,int,int);
	BOOL run() override;
	void Loop();
	virtual lClient* Accept();
};
#endif