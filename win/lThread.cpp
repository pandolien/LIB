#include "pch.h"
#include "win/lThread.h"

lThread::lThread()
{
	Initialization();
}

lThread::~lThread()
{
	Close();
}

void lThread::Initialization()
{
	id = 0;
	EndEvent = TRUE;
	this->dt = 0;
}

void lThread::Close()
{
	stop();
	id = 0;
	EndEvent = TRUE;
}
void lThread::start(unsigned int dt)
{
	if (EndEvent) {
		EndEvent = FALSE;
		this->dt = dt;
		hdr = CreateThread(NULL, NULL, RUN, this, 0, &id);
		if (hdr == NULL) {EndEvent = TRUE;}
		
	}

}

void lThread::stop()
{
	if (EndEvent)return;
	EndEvent = TRUE;
	DWORD result = WaitForSingleObject(hdr, 0);
	if (result == WAIT_OBJECT_0) {CloseHandle(hdr);}
}

DWORD WINAPI lThread::RUN(LPVOID lpParam)
{
	lThread* trd = (lThread*)lpParam;
	while (1) {
		if (trd->run())break;
	}
	return 0;
}

BOOL lThread::run()
{
	Sleep(dt);
	return EndEvent; 
}
