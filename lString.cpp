
#include "pch.h"
#include "lString.h"

lString::lString()
{
	Initialization();
}

lString::lString(const char str)
{
	Initialization();
	Init(str);
}

lString::lString(const char* str)
{
	Initialization();
	Init(str);
}

#ifdef _WIN64 
lString::lString(LPCTSTR lpctstr)
{
	n = WideCharToMultiByte(CP_ACP, 0, lpctstr, -1, nullptr, 0, nullptr, nullptr);
	str = new char[n + 1];
	memset(str, 0, n+1);
	WideCharToMultiByte(CP_ACP, 0, lpctstr, -1, str, n, NULL, NULL);
}
#elif _WIN32

lString::lString(LPCTSTR lpctstr)
{
	n = WideCharToMultiByte(CP_ACP, 0, lpctstr, -1, nullptr, 0, nullptr, nullptr);
	str = new char[n + 1];
	memset(str, 0, n + 1);
	WideCharToMultiByte(CP_ACP, 0, lpctstr, -1, str, n, NULL, NULL);
}
#endif
lString::lString(lString& str)
{
	Initialization();
	Init((const lString&)str);
	str.TempRemove();
}

lString::lString(const lString& str)
{
	Initialization(); 
	Init(str);
}

lString::~lString()
{
	if (isTemp())return;
	Delete();
}

void lString::Initialization()
{
	NoTemp();
	str = NULL;
	n = 0;
}

void lString::Init(const char str)
{
	Delete();
	this->str = new char[2];
	memset(this->str, 0, 2);
	this->str[0] = str;
}

void lString::Init(const char* str)
{
	int len;
	if (str == NULL)return;
	Delete();
	len = strlen(str);
	this->str = new char[len + 1];
	n = len;
	memset(this->str, 0, n+1);
	memmove(this->str, str, len);
}
void lString::Init(const lString& str)
{
	Delete();
	if (str.str == NULL)return;
	if (str.temp) {
		this->str = str.str;
		n = str.n;
		Temp();
	}
	else {
		this->str = new char[str.n + 1];
		memset(this->str, 0, str.n + 1);
		memmove(this->str, str.str, str.n);
		n = str.n;
		NoTemp();
	}
}

void lString::Init(unsigned int n)
{
	str = new char[n + 1];
	memset(str, 0, n + 1);
	this->n = n;
}

void lString::Init(FILE* fd, unsigned int n)
{
	str = new char[n + 1];
	fread(str, 1, n, fd);
	str[n] = 0;
	this->n = 0;
}

void lString::Format(const char*format, ...)
{
	va_list args;
	int n;
	char buf[1024];
	va_start(args, format);
	vsnprintf(buf, 1024, format, args);
	Init(buf);
	va_end(args);
}

void lString::Delete()
{
	if (str)delete[](str);
	str = NULL;
}

void lString::Read(const char* name)
{
	FILE* fp = NULL;
	long size;
	fp = fopen(name, "rb");
	if (fp == NULL) { return; }
	Delete();
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);   
	fseek(fp, 0,SEEK_SET);
	str = new char[size+1];
	memset(str, 0, size + 1);
	this->n = size;
	fread(str, 1, size,fp);
	fclose(fp);
}

void lString::RemoveChar(char c)
{
	if (str == NULL)return;
	char* p,*s,*Str;
	int i = 0, cnt = 0;
	unsigned int len;
	while (1) {
		if (str[i] == '\0')break;
		if (str[i] != c)cnt += 1;
		i += 1;
	}
	Str = new char[i+1];
	memset(Str, 0, i + 1);
	i = 0;
	cnt = 0;
	while (1) {
		if (str[i] == '\0')break;
		if (str[i] != c)Str[cnt++] = str[i];
		i += 1;
	}
	Delete();
	*this = Str;
	delete[](Str);
}

char* lString::GetC()
{
	NoTemp();
	return str;
}

long lString::GetLen()
{
	NoTemp();
	return n;
}

bool lString::In(lString str)
{
	char* p = strstr(GetC(), str.GetC());
	NoTemp();
	str.NoTemp();
	if(p)return true;
	return false;
}

lString lString::operator+(lString str)
{
	lString out;
	out.str = new char[n + str.GetLen()+1];
	out.n = n + str.GetLen();
	memset(out.str, 0, out.n+1);
	memmove(&out.str[0], this->str, n);
	memmove(&out.str[n], str.str, str.n);
	out.Temp();
	NoTemp();
	return out;
}

void lString::operator=(lString str)
{
	Delete();
	if (str.GetC() != NULL) {
		this->str = new char[str.GetLen() + 1];
		n = str.GetLen();
		memset(this->str, 0, str.GetLen() + 1);
		memmove(this->str, str.GetC(), str.GetLen());
	}
	str.TempRemove();
	NoTemp();
}

bool lString::operator==(lString str)
{
	int dt;
	if (str.str == NULL)return false;
	dt = strcmp(this->str, str.str);
	NoTemp();
	str.NoTemp();
	if (dt == 0)return true;
	return false;
}

bool lString::operator==(char* str)
{
	int dt = strcmp(this->str, str);
	NoTemp();
	if (dt == 0)return true;
	return false;
}

bool lString::operator!=(lString str)
{
	int dt = strcmp(this->str, str.str);
	NoTemp();
	str.NoTemp();
	if (dt == 0)return false;
	return true;
}

int lString::Int()
{
	NoTemp();
	return atoi(str);
}

float lString::Float()
{
	NoTemp();
	return atof(str);
}

lString lString::Left(char f)
{
	lString left;
	unsigned int len;
	char* result = strchr(str, f);
	if (result == NULL)return left;
	len = (unsigned int)(result-str);
	left.Init(len);
	memmove(left.str, str, sizeof(char) * len);
	left.Temp();
	return left;
}

lString lString::Right(char f)
{
	lString right;
	char* result = strchr(str, f);
	if (result == NULL)return right;
	result++;
	right = (const char*)result;
	right.Temp();
	return right;
}
