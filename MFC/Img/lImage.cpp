#include "pch.h"
#include "lImage.h"

lImage::lImage()
{
	this->Initialization();
}

lImage::lImage(lImage& img)
{
	this->Initialization();
	Init((const lImage&)img);
}
lImage::lImage(const lImage& img)
{
	this->Initialization();
	Init(img);
}

lImage::~lImage()
{
	if (isTemp()) { return; }
	Close();
}

void lImage::Initialization()
{
	this->buf = NULL;
	h = 0;
	w = 0;
	bpp = 0;
	NoTemp();
}

void lImage::Init(int h, int w)
{
	Close();
	this->h = h;
	this->w = w;
	this->bpp = 1;
	buf = new unsigned char[h*w];
	memset(buf, 0, h * w);
}

void lImage::Init(int h, int w, int c)
{
	Close();
	this->h = h;
	this->w = w;
	this->bpp = c;
	buf = new unsigned char[h * w * c];
	memset(buf, 0, h * w * c);
	
}

void lImage::Init(const lImage& img)
{
	Close();
	if (img.temp) {
		this->h = img.h;
		this->w = img.w;
		this->bpp = img.bpp;
		this->buf = img.buf;
		Temp();
	}
	else {
		this->h = img.h;
		this->w = img.w;
		this->bpp = img.bpp;
		Init(h, w, bpp);
		memmove(buf, img.buf, sizeof(char) * h * w * bpp);

	}
	NoTemp();
}

void lImage::Close()
{
	if (this->buf)delete[](buf);
	buf = NULL;
	h = 0;
	w = 0;
	bpp = 0;
}

BOOL lImage::isTemp()
{
	return temp;
}

void lImage::Temp()
{
	temp = true;
}

void lImage::NoTemp()
{
	temp = false;
}

void lImage::TempRemove()
{
	if (isTemp()) {
		NoTemp();
		Close();
	}
}

void lImage::Load(const char* name)
{
	CImage img;
	CString str = CString(name);
	int i, j;
	unsigned char* pixel,*base;
	img.Load(str);
	h = img.GetHeight();
	w = img.GetWidth();
	bpp = img.GetBPP()/8;
	int pitch = img.GetPitch();
	base = (unsigned char*)img.GetBits();
	buf = new unsigned char[h*w*bpp];
	
	for (i = 0; i < h; i++) {
		pixel  = base + i * pitch;
		for (j = 0; j < w; j++) {
			if (bpp == 3) {
				buf[(i * w + j) * 3 + 0] = pixel[j * 3 + 2];
				buf[(i * w + j) * 3 + 1] = pixel[j * 3 + 1];
				buf[(i * w + j) * 3 + 2] = pixel[j * 3 + 0];
			}
			else if(bpp == 4){
				buf[(i * w + j) * 4 + 0] = pixel[j * 4  + 2];
				buf[(i * w + j) * 4 + 1] = pixel[j * 4  + 1];
				buf[(i * w + j) * 4 + 2] = pixel[j * 4  + 0];
				buf[(i * w + j) * 4 + 3] = pixel[j * 4  + 3];
			}
		}
	}
	NoTemp();
}

lImage lImage::GetNeighborPixel(int u, int v, int h, int w)
{
	lImage out;
	int i,j,k;
	if (u < 0) {
		out.Temp();
		return out;
	}
	if (u >= this->w) {
		out.Temp();
		return out;
	}
	if (v < 0) {
		out.Temp();
		return out;
	}
	if (v >= this->h) {
		out.Temp();
		return out;
	}
	out.Init(h, w, this->bpp);
	for (i = 0; i < h; i++) {
		for (j = 0; j < w; j++) {
			int x, y;
			x = u + i - (int)((float)(w) / 2 + 0.5);
			y = v + j - (int)((float)(h) / 2 + 0.5);
			if (x < 0)continue;
			if (x >= this->w)continue;
			if (y < 0)continue;
			if (y >= this->h)continue;
			
			memmove(&out.buf[(i * w + j) * bpp], &buf[(y * this->w + x) * bpp], bpp);
		}
	}
	out.Temp();
	NoTemp();
	return out;
}

lImage lImage::GrayScail()
{
	lImage out;
	int i, j;
	out.Init(this->h, this->w);
	for (i = 0; i < this->h; i++) {
		for (j = 0; j < this->w; j++) {
			out.In(i, j) = Out(i, j, 0) * 0.299+ Out(i, j, 1) * 0.587+ Out(i, j, 2)*0.114;
		}
	}
	out.Temp();
	return out;
}

unsigned char& lImage::In(int i , int j , int k)
{
	NoTemp();
	return buf[(i * w + j)*bpp + k];
}

unsigned char& lImage::In(int i, int j)
{
	NoTemp();
	return buf[(i * w + j) * bpp];
}

unsigned char lImage::Out(int i, int j , int k)
{
	NoTemp();
	if (buf == NULL)return 0;
	return buf[(i*w+j) * bpp +k];
}

void lImage::FAST(lPTRList*list , float trd, int cnt)
{
	list->RemoveAll();
	int i, j,k;
	lImage Neighber;
	for (i = 0; i < this->h; i++) {}
	return;
}



