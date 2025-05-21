#ifndef __LOBJECT_H__
#define __LOBJECT_H__
#include "Obj/lPoly.h"
#include "Obj/lObj.h"

#include "3d/dhVertex.h"
#include "3d/dhVector.h"
#include "3d/dhMat.h"


#include "lString.h"
#include "Obj/ObjectNode.h"
class vObject {
public:
	vObject();
	~vObject();
public:
	void Initialization();
	void Close();
public:
	NODE_OBJECT*	root;
	dhMat O;
	lPTRList list;
	void SetCamAndLight(void*,void*);
	void SetShader(const char*, const char*);
	void Draw();
	void Read(lString name);
private://GLB Read Code
	NODE_OBJECT* GetNode(const char*);
	void Read_GLB_Check(const char*);
		void Read_GLB_ObjectNode(void*);
			BOOL Read_GLB_ObjectMesh(int,_GLB*, void*);
				dhVertex* Read_GLB_SetVertexPoint(void*, int*);
				lPoly* Read_GLB_SetIndexData(void*, int*);
		BOOL Read_GLB_TreeData(void*);
private://3DS Read Code
	int GetName(FILE*, _3DS*);
	void Read_3DS_Check(FILE*fd);
		void Read_3DS_ObjectFileRead(FILE*,int);
			void Read_3DS_ObjectNodeMake(FILE*,int);
				void Read_3DS_ObjectNode(FILE*, int);
					void Read_3DS_ObjectDataRead(_3DS*,FILE*,int);
				void Read_3DS_ObjectColorData(FILE*, int);
					lString Read_3DS_ObjectColorName(FILE*,int);
					void Read_3DS_ObjectColorData(FILE* ,lRGB&,int);
				void Read_3DS_ObjectHMatrixData(FILE*, int);
					void Read_3DS_ObjectMeshInfo(FILE*, int);
						_3DS* Read_3DS_ObjectMaterialName(FILE*);
						void Read_3DS_ObjectPIVOT(FILE*, _3DS*);
						void Read_3DS_ObjectPositionData(FILE*, _3DS*, unsigned int);
						void Read_3DS_ObjectQuaternionData(FILE*, _3DS*, unsigned int);
						void Read_3DS_ObjectScailData(FILE*, _3DS*, unsigned int);
					
};
#endif
