#include "pch.h"
#include "lPTRList.h"
lPTRList::lPTRList() {
	Initialization();
}
lPTRList::~lPTRList() {
	RemoveAll();
}
void lPTRList::Initialization() {
	this->Del = true;
	this->n = 0;
	this->SearchNumber = -1;
	this->pFind = NULL;
	this->pHead = NULL;
	this->pTail = NULL;
}
void lPTRList::Init() {
	
}
void lPTRList::Delete(BLOCK *D)
{
	if (D == NULL) return;
	if (Del) {
		if(D->pData)delete(D->pData);
	}
	D->pData = NULL;
}
void lPTRList::DeleteModeOff() { Del = false; }
void lPTRList::DeleteModeOn() { Del = true; }
void lPTRList::RemoveAll() {
	if (pHead == NULL && pTail == NULL)return;
	BLOCK* pTep;
	while (1) {
		pTep = pHead->pNext;
		if (pTep == NULL)break;
		pTep->pPerv = NULL;
		pHead->pNext = NULL;
		Delete(pHead);
		delete(pHead);
		pHead = pTep;
	}
	Delete(pHead);
	delete(pHead);
	pHead = NULL;
	pTail = NULL;
	pFind = NULL;
	n = 0;
	SearchNumber = -1;
}
void lPTRList::RemoveN(int n) {
	BLOCK *pPerv, *pNext;
	if (pHead == NULL && pTail == NULL)return;
	Search(n);
	if (pFind == NULL)return;
	pPerv = pFind->pPerv;
	pNext = pFind->pNext;
	if (pPerv == NULL &&pNext == NULL) {
		Delete(pFind);
		delete(pFind);
		pFind = NULL;
		pHead = NULL;
		pTail = NULL;
		this->n = 0;
		SearchNumber = -1;
	}
	else if (pPerv != NULL && pNext == NULL) {
		pTail->pPerv = NULL;
		pPerv->pNext = NULL;
		Delete(pTail);
		delete(pTail);
		pTail = pPerv;
		this->n -= 1;
	}
	else if (pPerv == NULL && pNext != NULL) {
		pHead->pNext = NULL;
		pNext->pPerv = NULL;
		Delete(pHead);
		delete(pHead);
		pHead = pNext;
		this->n -= 1;
	}
	else {
		pFind->pNext = NULL;
		pFind->pPerv = NULL;
		Delete(pFind);
		delete(pFind);
		this->n -= 1;
		pNext->pPerv = pPerv;
		pPerv->pNext = pNext;
	}
	pFind = NULL;
	SearchNumber = -1;
}
void lPTRList::AddTail(void *pData) {
	BLOCK* New = new BLOCK;
	New->pNext = NULL;
	New->pPerv = NULL;
	New->pData = pData;
	if (pHead == NULL)pHead = New;
	if (pTail) {
		pTail->pNext = New;
		New->pPerv = pTail;
		pTail = New;
	}
	else pTail = New;
	n += 1;
}
void lPTRList::Change(int a, int b)
{
	BLOCK* p1, * p2;
	void* Dt;
	Search(a);
	p1 = pFind;
	Search(b);
	p2 = pFind;
	Dt = p1->pData;
	p1->pData = p2->pData;
	p2->pData = Dt;

}
int lPTRList::GetSize() {
	return n;
}
void* lPTRList::GetAt(int n)
{
	BLOCK* pTep;
	int cnt = 1;
	if (pHead == NULL && pTail == NULL)return NULL;
	if (n >= this->n)return NULL;
	if (n < 0)return NULL;
	if (n == 0) { 
		pFind = pHead;
		SearchNumber = 0;
		return pHead->pData; 
	}
	if (n == this->n - 1) {
		pFind = pTail;
		SearchNumber = n;
		return pTail->pData;
	}
	pTep = pHead->pNext;
	while (1) {
		if (n == cnt)break;
		pTep = pTep->pNext;
		cnt += 1;
	}
	pFind = pTep;
	SearchNumber = cnt;
	return pTep->pData;
}
void* lPTRList::Search(int n) {
	int diff;
	if (pHead == NULL && pTail == NULL)return NULL;
	if (SearchNumber == -1||pFind == NULL)return GetAt(n);
	if (n >= this->n)return NULL;
	if (n < 0)return NULL;
	diff = SearchNumber - n;
	
	if (diff == 0) { return pFind->pData; }
	else if (diff == -1) {
		pFind = pFind->pNext;
		SearchNumber += 1;
		return pFind->pData;
	}
	else if (diff == 1) {
		pFind = pFind->pPerv;
		SearchNumber -= 1;
		return pFind->pData;
	}
	else { return GetAt(n); }
	return NULL;
}