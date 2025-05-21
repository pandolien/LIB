#include "pch.h"
#include "graph.h"

graph::graph()
{
	Initialzation();
}

graph::~graph()
{
	Delete();
}

void graph::Initialzation()
{
	list.Initialzation();
	list.DeleteModeOn();
}

void graph::Delete()
{
	int i;
	for (i = 0; i < list.GetSize(); i++) {
		graphNode* nd = (graphNode*)list.Search(i);
		nd->Delete();
	}
	list.RemoveAll();
}

void graph::AddNode(int n, dhVector o)
{
	graphNode* node = new graphNode();
	node->Initialzation();
	node->id = n;
	node->v = o;
	list.AddTail(node);
}

void graph::CLink(int a, int b)
{
	graphNode* node1 = (graphNode*)Search(a);
	graphNode* node2 = (graphNode*)Search(b);
	node1->Link(node2);
	node2->Link(node1);
}

void graph::Link(int a, int b)
{
	graphNode* node1 = (graphNode*)Search(a);
	graphNode* node2 = (graphNode*)Search(b);
	node1->Link(node2);
}

void* graph::Search(int id)
{
	int i;
	for (i = 0; i < list.GetSize(); i++) {
		graphNode* node = (graphNode*)list.Search(i);
		if (node->id == id)return node;
	}
	return NULL;
}

int* graph::FindLode(int s, int e)
{
	int *load = NULL, result,i;
	lPTRList reached,List;
	reached.DeleteModeOff();
	List.DeleteModeOff();
	graphNode* nd = (graphNode*)Search(s);
	result = nd->FindLoad(e, &List, &reached);
	
	if (result == 1) {
		load = new int[List.GetSize() ];
		for (i = 0; i < List.GetSize(); i++) {
			nd = (graphNode*)List.Search(i);
			load[i] = nd->id;
		}
	}
	List.RemoveAll();
	reached.RemoveAll();
	return load;
}

graphNode::graphNode()
{
	Initialzation();
}

graphNode::~graphNode()
{
	Delete();
}

void graphNode::Initialzation()
{
	list.Initialzation();
	list.DeleteModeOff();
}

void graphNode::Delete()
{
	list.RemoveAll();
}

void graphNode::Link(graphNode* n)
{
	list.AddTail(n);
}

int graphNode::FindLoad(int e, lPTRList* List, lPTRList*reached)
{
	int i,j,result,sID = -1;
	float sDis = 1e10 , nDis;
	graphNode* nd,*nd2;
	lPTRList* ndlist,*sndlist = NULL;
	if (id == e) {
		List->AddTail(this);
		return 1;
	}
	for (i = 0; i < reached->GetSize(); i++) {
		nd = (graphNode*)reached->Search(i);
		if (nd == this) return 0;
	}
	reached->AddTail(this);
	for (i = 0; i < list.GetSize(); i++) {
		ndlist = new lPTRList();
		ndlist->Initialzation();
		ndlist->DeleteModeOff();
		ndlist->AddTail(this);
		nd = (graphNode*)list.Search(i);
		result = nd->FindLoad(e, ndlist, reached);
		if (result == 0) {
			ndlist->RemoveAll();
			delete(ndlist);
			continue;
		}
		nDis = 0;
		for (j = 0; j < ndlist->GetSize()-1;j++) {
			nd = (graphNode*)ndlist->Search(j);
			nd2 = (graphNode*)ndlist->Search(j+1);
			nDis += nd->v.Norm(nd2->v);
		}
		if (sDis > nDis) {
			if (sndlist) {
				sndlist->RemoveAll();
				delete(sndlist);
				sndlist = NULL;
			}
			sndlist = ndlist;
			sDis = nDis;
		}
		else {
			ndlist->RemoveAll();
			delete(ndlist);
		}
	}
	reached->RemoveN(reached->GetSize() - 1);
	if (sndlist) {
		for (i = 0; i < sndlist->GetSize(); i++) {
			List->AddTail(sndlist->Search(i));
		}
		sndlist->RemoveAll();
		delete(sndlist);

		return 1;
	}
	return 0;
}
