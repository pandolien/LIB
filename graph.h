#ifndef __GRAPH_H__
#define __GRAPH_H__
#include "lPTRList.h"
#include "dhVector.h"
class graph {
public:
	graph();
	~graph();
public:
	void Initialzation();
	void Delete();
public:
	lPTRList list;
public:
	void AddNode(int n, dhVector);
	void CLink(int a, int b);
	void Link(int a, int b);
	void* Search(int id);
	int* FindLode(int s, int e);
};
class graphNode {
public:
	graphNode();
	~graphNode();
public:
	void Initialzation();
	void Delete();
public:
	lPTRList list;
	int id;
	dhVector v;
public:
	void Link(graphNode*);
	int FindLoad(int, lPTRList*, lPTRList*);
};

#endif

