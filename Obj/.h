#ifndef __LT3DNODE_H__
#define __LT3DNODE_H__
#include "lPTRList.h"
class lt3dNode : public lPTRList
{
public:
    lt3dNode();
    ~lt3dNode();
public:
    void Delete(BLOCK* D);
    void* SearchNode(const char* name);
};
#endif
