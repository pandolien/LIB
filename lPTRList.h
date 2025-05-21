#ifndef __LPTRLIST_H__
#define __LPTRLIST_H__
typedef struct _BLOCK {
	_BLOCK* pPerv;
	_BLOCK* pNext;
	void* pData;
}BLOCK;
class lPTRList
{
public:
	lPTRList();
	~lPTRList();
public:
	_BLOCK* pHead;
	_BLOCK* pTail;
	_BLOCK* pFind;
	int n;
	int SearchNumber;
	bool Del;
public:
	void Initialization();
	void Init();
	virtual void Delete(BLOCK* D);
public:
	void DeleteModeOn();
	void DeleteModeOff();
	void RemoveAll();
	void RemoveN(int);
	void AddTail(void*);
	void Change(int, int);
public://return
	int GetSize();
	void* GetAt(int);
	void* Search(int);
};
#endif

