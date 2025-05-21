#ifndef __LPOLY_H__
#define __LPOLY_H__
class lPoly{
public:
	lPoly();
	~lPoly();
public:
	void Initialization();
	void Close();
public:
	unsigned short f, s, t;
public:
	void SetPoly(short, short, short);
};

#endif