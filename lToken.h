#ifndef __LTOKEN_H__
#define __LTOKEN_H__
#include <string.h>
#include "lPTRList.h"
#include "lString.h"
class lToken
{
public:
	lToken();
	~lToken();
public:
	lPTRList list;
public:
	void Initialization();
	void Delete();
public:
	void Split(lString,char*);
	lString Search(int);
	int GetSize();
	bool In(lString);
};
#endif
