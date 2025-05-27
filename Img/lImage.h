#ifndef __IMG_H__
#define __IMG_H__
#include <atlimage.h>
#include <3d/dhVector.h>
#include "lPTRList.h"
typedef struct {
	int u, v,ch;
	float angle;
}IMG_LOCATION;
class lImage {
public:
	lImage();
	lImage(lImage&);
	lImage(const lImage&);
	~lImage();
public:
	void Initialization();
	void Init(int, int);
	void Init(int, int,int);
	void Init(const lImage&);
	void Close();
private:
	unsigned char* buf;
	int h, w;
	int bpp;
public:
	BOOL temp;
	BOOL isTemp();
	void Temp();
	void NoTemp();
	void TempRemove();
public:
	void Load(const char*);
	lImage GetNeighborPixel(int,int,int,int);
	lImage GrayScail();
	unsigned char& In(int, int, int);
	unsigned char& In(int, int);
	unsigned char Out(int, int, int);
public:
	unsigned char* ConvertGDIFormat( );
	void Draw(CDC*, int x, int y);
public:
	void FAST(lPTRList*,unsigned char th = 20,int cnt = 12);
	void OrientedFAST(lPTRList*, unsigned char th = 20, int cnt = 12);
};
#endif
