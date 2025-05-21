#ifndef __LSTRING_H__
#define __LSTRING_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

class lString
{
public:
	lString();
	lString(const char);
	lString(const char*);
#ifdef _WIN64 
	lString(LPCTSTR);
#elif _WIN32
	lString(LPCTSTR);
#endif
	lString(lString&);
	lString(const lString&);
	
	~lString();
public:
	char* str;
	long n;
public:
	bool temp;
	bool isTemp() {return temp;}
	void Temp() {temp = true;}
	void NoTemp() {temp = false;}
	void TempRemove() {
		if (isTemp()) {
			NoTemp();
			Delete();
		}
	}
public:
	void Initialization();
	void Init(const char);
	void Init(const char*);
	void Init(const lString&);
	void Init(unsigned int);
	void Init(FILE*, unsigned int);
	void Format(const char*, ...);
	void Delete();
	void Read(const char*);
	void RemoveChar(char);
public:
	char* GetC();
	long GetLen();
public:
	bool In(lString);
	lString operator +(lString);
	void operator =(lString);
	bool operator ==(lString);
	bool operator ==(char*);
	bool operator !=(lString);
public:
	int Int();
	float Float();
	lString Left(char);
	lString Right(char);
};

#endif