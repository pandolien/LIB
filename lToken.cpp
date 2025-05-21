#include "pch.h"
#include "lToken.h"
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

lToken::lToken()
{
	Initialization();
}

lToken::~lToken()
{
	Delete();
}

void lToken::Initialization()
{
	list.Initialization();
}

void lToken::Delete()
{
	int i;
	for (i = 0; i < list.GetSize(); i++) {
		lString* str = (lString*)list.Search(i);
		str->Delete();
	}
	list.RemoveAll();
}

void lToken::Split(lString str, char* Dt)
{
	lString* tok;
	Delete();
	char *ctok = strtok(str.GetC(), Dt);
	tok = new lString();
	tok->Initialization();
	tok->Init(ctok);
	list.AddTail(tok);
	while (1) {
		ctok = strtok(NULL, Dt);
		if (ctok == NULL)break;
		tok = new lString();
		tok->Initialization();
		tok->Init(ctok);
		list.AddTail(tok);
	}
	str.NoTemp();
}

lString lToken::Search(int i)
{
	lString* str = (lString*)list.Search(i);
	lString out;
	if (str == NULL) return out;
	out = *str;
	out.Temp();
	return out;
}

int lToken::GetSize()
{
	return list.GetSize();
}

bool lToken::In(lString Str)
{
	int i;
	lString str;
	if (list.GetSize() == 0)return false;
	for (i = 0; i < list.GetSize(); i++) {
		str = Search(i);
		if (str == Str) return true;
	}
	Str.NoTemp();
	return false;
}
