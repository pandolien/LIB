#ifndef __LTHREAD_H__
#define __LTHREAD_H__
#include "windows.h"
class lThread
{
public://constructor
	lThread();
	~lThread();
public://Initialization and Close Function
	virtual void Initialization();
	virtual void Close();
public://Thread variable
	DWORD id;//Thread ID variable
	HANDLE hdr;//Thread handler
	BOOL EndEvent;//Thread Stop variable
	unsigned int dt;//ms
public://Thread Control Funcction
	void start(unsigned int dt);
	void stop();
	static DWORD WINAPI RUN(LPVOID lpParam);
	virtual BOOL run();
};

#endif