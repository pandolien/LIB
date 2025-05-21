#include "pch.h"

#define CGLTF_IMPLEMENTATION
#include "Obj/cgltf.h"
#include "Obj/lObject.h"
#include "lToken.h"

vObject::vObject()
{
	Initialization();
}

vObject::~vObject()
{
	Close();
}

void vObject::Initialization()
{
	root = NULL;
	list.Initialization();
	list.DeleteModeOff();
}

void vObject::Close()
{
	int i;
	root = NULL;
	if (list.GetSize() != 0) {
		for (i = 0; i < list.GetSize(); i++) {
			NODE_OBJECT* obj = (NODE_OBJECT*)list.Search(i);
			obj->Close();
			delete(obj);
		}
		list.RemoveAll();
	}
}

void vObject::SetCamAndLight(void* c, void* l)
{
	int i;
	for (i = 0; i < list.GetSize(); i++) {
		NODE_OBJECT* obj = (NODE_OBJECT*)list.Search(i);
		obj->SetCamAndLight(c,l);
	}
}

void vObject::SetShader(const char*vertex, const char*fragment)
{
	int i;
	for (i = 0; i < list.GetSize(); i++) {
		NODE_OBJECT* obj = (NODE_OBJECT*)list.Search(i);
		obj->SetShader(vertex, fragment);
	}
}

void vObject::Draw()
{
	int i;
	if (root)root->Draw(O);
	else {
		for (i = 0; i < list.GetSize(); i++) {
			NODE_OBJECT* obj = (NODE_OBJECT*)list.Search(i);
			obj->Draw(O);
		}
	}
}

void vObject::Read(lString name)
{
	lToken tok;
	lString FileType;
	tok.Split(name, ".");
	FileType = tok.Search(1);
	if (FileType == "3ds" || FileType == "3DS") {
		FILE* fd = fopen((const char*)name.GetC(), "rb");
		Read_3DS_Check(fd);
		fclose(fd);
	}
	else if (FileType == "glb" || FileType == "GLB") {
		Read_GLB_Check((const char*)name.GetC());
	}
	else {
	}
}

NODE_OBJECT* vObject::GetNode(const char* name){
	int i;
	for (i = 0; i < list.GetSize(); i++) {
		NODE_OBJECT* obj = (NODE_OBJECT*)list.Search(i);
		if (obj->strName == name) return obj;
	}
	return NULL;
}

void vObject::Read_GLB_Check(const char* filename)
{
	cgltf_options options = {};
	cgltf_data* data = NULL;
	cgltf_result result = cgltf_parse_file(&options, filename, &data);
	if (result != cgltf_result_success) {return;}
	result = cgltf_load_buffers(&options, data, filename);
	if (result != cgltf_result_success) {
		cgltf_free(data);
		return;
	}
	Read_GLB_ObjectNode(data);
	cgltf_free(data);
	return;
}

void vObject::Read_GLB_ObjectNode(void*dt){
	cgltf_data* data = (cgltf_data*)dt;
	cgltf_node* nd;
	int i;
	if (data == NULL)return;
	for (i = 0; i < data->nodes_count; i++) {
		nd = &data->nodes[i];
		_GLB* node = new _GLB();
		node->Initialization();
		list.AddTail(node);
		if (Read_GLB_ObjectMesh(i, node, nd))continue;
		Close();
		return;
	}
	for (i = 0; i < data->nodes_count; i++) {
		nd = &data->nodes[i];
		this->Read_GLB_TreeData(nd);
	}
}

BOOL vObject::Read_GLB_ObjectMesh(int n,_GLB*object, void*Nd){
	cgltf_node* nd = (cgltf_node*)Nd;
	int i;
	dhVertex* vtx;
	lPoly* ply;
	int vtn, pln;
	cgltf_mesh* ms;
	if (nd->name) object->strName = nd->name;
	else { object->strName.Format("Unnamed%d",n); }
	if (nd->mesh) { 
		ms = nd->mesh;
		object->Init(ms->primitives_count);
		for (i = 0; i < ms->primitives_count; i++) {
			vtx = Read_GLB_SetVertexPoint(&ms->primitives[i],&vtn);
			ply = Read_GLB_SetIndexData(&ms->primitives[i], &pln);
			if (vtx && ply == NULL) {
				delete[](vtx);
				return FALSE;
			}
			object->mesh[i].SetBuffer(vtx, vtn, ply, pln);
			if (ply)delete[](ply);
			if (vtx)delete[](vtx);
		}
	}
	
	return TRUE;
}

dhVertex* vObject::Read_GLB_SetVertexPoint(void* Primitive,int *vtn)
{
	cgltf_primitive* primitive = (cgltf_primitive*)Primitive;
	dhVertex* vtx = NULL;
	int i,j;
	for (i = 0; i < primitive->attributes_count; i++) {
		cgltf_attribute* attribute = &primitive->attributes[i];
		if (attribute->type != cgltf_attribute_type_position) continue;
		cgltf_accessor* accessor = attribute->data;
		cgltf_buffer_view* buffer_view = accessor->buffer_view;
		cgltf_buffer* buffer = buffer_view->buffer;
		size_t offset = buffer_view->offset + accessor->offset;
		const uint8_t* base = (const uint8_t*)buffer_view->buffer->data;
		//float* buffer_data = (float*)buffer_view->buffer->data + buffer_view->offset + accessor->offset;
		const float* buffer_data = (const float*)(base + offset);
		*vtn = accessor->count;
		if (*vtn == 0)return NULL;
		vtx = new dhVertex[*vtn];
		for (j = 0; j < *vtn; j++) {
			vtx[j].x = buffer_data[j * 3 + 0];
			vtx[j].y = buffer_data[j * 3 + 1];
			vtx[j].z = buffer_data[j * 3 + 2];
		}
		return vtx;
	}
	return NULL;
}

lPoly* vObject::Read_GLB_SetIndexData(void* Primitive, int* pln)
{
	cgltf_primitive* primitive = (cgltf_primitive*)Primitive;
	lPoly* ply;
	int i;
	cgltf_accessor* accessor;
	cgltf_buffer_view* buffer_view;
	if (primitive->indices) {
		accessor = primitive->indices;
		buffer_view = accessor->buffer_view;
		if (accessor->component_type == cgltf_component_type_r_32u) return NULL;
		else {
			unsigned char* buffer_data = (unsigned char*)buffer_view->buffer->data + buffer_view->offset + accessor->offset;
			*pln = (int)accessor->count/3;
			ply = new lPoly[*pln];
			for (i = 0; i < *pln; i++) {
				ply[i].Initialization();
				if (accessor->component_type == cgltf_component_type_r_16u) {
					unsigned short* buf = (unsigned short*)buffer_data;
					ply[i].SetPoly(	buf[i * 3 + 0],
												buf[i * 3 + 1],
												buf[i * 3 + 2]);
				}
				else {
					ply[i].SetPoly(	(short)buffer_data[i * 3 + 0],
												(short)buffer_data[i * 3 + 1],
												(short)buffer_data[i * 3 + 2]);
				}
			}
		}
	}
	return ply;
}

BOOL vObject::Read_GLB_TreeData(void* Nd){
	cgltf_node* nd = (cgltf_node*)Nd;
	_GLB* obj,*parent= NULL;
	obj = (_GLB*)GetNode(nd->name);
	if(nd->parent)parent = (_GLB*)GetNode(nd->parent->name);
	else {
		root = obj;
	}
	if(obj)obj->parents = parent;
	if (parent)parent->AddTail(obj);
	if (nd->translation) {obj->SetTranslation(nd->translation);}
	if (nd->rotation) obj->SetQuat(nd->rotation);
	if (nd->scale)obj->SetScale(nd->scale);
	return TRUE;
}

int vObject::GetName(FILE*fd, _3DS* obj)
{
	int i = 0;
	char c[500] = { 0, };
	while (1) {
		fread(&c[i], 1, 1, fd);
		if (c[i] == '\0')break;
		i += 1;
	}
	obj->strName = (const char*)c;
	return i;
}

void vObject::Read_3DS_Check(FILE* fd)
{
	unsigned short ID;
	unsigned int len;
	fread(&ID, sizeof(short), 1, fd);
	fread(&len, sizeof(int), 1, fd);
	if (ID == 0x4D4D) {
		Read_3DS_ObjectFileRead(fd,len-6);
	}

}

void vObject::Read_3DS_ObjectFileRead(FILE*fd,int size)
{
	unsigned short ID;
	unsigned int len;
	while (1) {
		fread(&ID, sizeof(short), 1, fd);
		fread(&len, sizeof(int), 1, fd);
		size -= 6;
		if (ID == 0x3D3D) {
			Read_3DS_ObjectNodeMake(fd, len - 6);
		}
		else if (ID == 0xAFFF) {
			Read_3DS_ObjectColorData(fd, len - 6);
		}
		else if (ID == 0xB000) {
			Read_3DS_ObjectHMatrixData(fd, len - 6);
		}
		else {
			fseek(fd, len-6, SEEK_CUR);
		}
		size -= (len - 6);
		if (size <= 0)break;
	}
}

void vObject::Read_3DS_ObjectNodeMake(FILE* fd, int size)
{
	unsigned short ID;
	unsigned int len;
	while (1) {
		fread(&ID, sizeof(short), 1, fd);
		fread(&len, sizeof(int), 1, fd);
		size -= 6;
		if (ID == 0x4000) {
			Read_3DS_ObjectNode(fd, len - 6);
		}
		else {
			fseek(fd, len - 6, SEEK_CUR);
		}
		size -= (len - 6);
		if (size <= 0)break;
	}
}

void vObject::Read_3DS_ObjectNode(FILE*fd, int size)
{
	unsigned short ID;
	unsigned int len;
	_3DS* obj = new _3DS();
	obj->Initialization();
	size -= (GetName(fd, obj)+1);
	size = size;
	while (1) {
		fread(&ID, sizeof(short), 1, fd);
		fread(&len, sizeof(int), 1, fd);
		size -= 6;
		if (ID == 0x4100) {
			Read_3DS_ObjectDataRead(obj, fd, len - 6);
		}
		else {
			fseek(fd, len - 6, SEEK_CUR);
		}
		size -= (len - 6);
		if (size <= 0)break;
	}
	list.AddTail(obj);
}

void vObject::Read_3DS_ObjectDataRead(_3DS*obj, FILE*fd, int size)
{
	unsigned short ID, datasize,f,s,t;
	unsigned int len;
	dhVector u,v,w,origin;
	dhVertex* vtx= NULL;
	lPoly* ply = NULL;
	int vtn, pln,i;
	while (1) {
		fread(&ID, sizeof(short), 1, fd);
		fread(&len, sizeof(int), 1, fd);
		size -= 6;
		if (ID == 0x4110) {//vertex
			fread(&datasize, sizeof(short), 1, fd);
			vtn = datasize;
			if (datasize != 0) {
				vtx = new dhVertex[datasize];
				for (i = 0; i < datasize; i++) {
					fread(&v, sizeof(dhVector), 1, fd);
					vtx[i].Initialization();
					vtx[i].x = v.x;
					vtx[i].y = v.y;
					vtx[i].z = v.z;
				}
			}
		}
		else if (ID == 0x4120) {//Index
			fread(&datasize, sizeof(short), 1, fd);
			pln = datasize;
			if (datasize > 0) {
				ply = new lPoly[datasize];
				for (i = 0; i < datasize; i++) {
					fread(&f, sizeof(short), 1, fd);
					fread(&s, sizeof(short), 1, fd);
					fread(&t, sizeof(short), 1, fd);
					ply[i].Initialization();
					ply[i].SetPoly(f, s, t);
					fread(&t, sizeof(short), 1, fd);
				}
			}
		}
		else if (ID == 0x4160) {//Matrix
			fread(&u, sizeof(dhVector), 1, fd);
			fread(&v, sizeof(dhVector), 1, fd);
			fread(&w, sizeof(dhVector), 1, fd);
			fread(&origin, sizeof(dhVector), 1, fd);
		}
		else {
			fseek(fd, len - 6, SEEK_CUR);
		}
		size -= (len - 6);
		if (size <= 0)break;
	}
	obj->mesh.SetBuffer(vtx, vtn, ply, pln);
	obj->SetLCS(u, v, w, origin);
	if(vtx)delete[](vtx);
	if(ply)delete[](ply);
}

void vObject::Read_3DS_ObjectColorData(FILE* fd, int size)
{
	unsigned short ID;
	unsigned int len;
	int i;
	lString str;
	_3DS* obj = NULL;
	while (1) {
		fread(&ID, sizeof(short), 1, fd);
		fread(&len, sizeof(int), 1, fd);
		size -= 6;
		if (ID == 0xA000) {
			str = Read_3DS_ObjectColorName(fd, len - 6);
			for (i = 0; i < list.GetSize(); i++) {
				_3DS *OBJ = (_3DS*)list.Search(i);
				if (str == OBJ->strName) {
					obj = OBJ;
					break;
				}
			}
			size -= (len - 6);
		}
		else if (ID == 0xA010) {
			if (obj == NULL)return;
			Read_3DS_ObjectColorData(fd, obj->Ambient, len-6);
			size -= (len - 6);
		}
		else if (ID == 0xA020) {
			if (obj == NULL)return;
			Read_3DS_ObjectColorData(fd, obj->Diffues, len - 6);
			size -= (len - 6);
		}
		else if (ID == 0xA030) {
			if (obj == NULL)return;
			Read_3DS_ObjectColorData(fd, obj->Specular, len - 6);
			size -= (len - 6); 
		}
		else {
			fseek(fd, len - 6, SEEK_CUR);
			size -= (len - 6);
		}
		if (size <= 0)break;
	}

}

lString vObject::Read_3DS_ObjectColorName(FILE*fd, int size)
{
	int i = 0;
	char c[500] = {0,};
	lString out;
	while (1) {
		fread(&c[i], 1, 1, fd);
		if (c[i] == '\0') break;
		i+=1;
	}
	out = (const char*)c;
	out.Temp();
	return out;
}

void vObject::Read_3DS_ObjectColorData(FILE* fd, lRGB& color, int size)
{
	unsigned short ID;
	unsigned int len;
	float vfColor;
	BYTE vbR, vbG, vbB;
	while (1) {
		fread(&ID, sizeof(short), 1, fd);
		fread(&len, sizeof(int), 1, fd);
		size -= 6;
		if (ID == 0x0010 || ID == 0x0011) {
			fread(&vfColor, sizeof(float), 1, fd);
			color.r = vfColor;
			fread(&vfColor, sizeof(float), 1, fd);
			color.g = vfColor;
			fread(&vfColor, sizeof(float), 1, fd);
			color.b = vfColor;
			color.d = 1;
			size -= (len - 6);
		}
		else if (ID == 0x0012 || ID == 0x0013) {
			fread(&vbR, 1, 1, fd);
			fread(&vbG, 1, 1, fd);
			fread(&vbR, 1, 1, fd);
			color.SetRGBD(vbR, vbG, vbB, 255);
			size -= (len - 6);
		}
		else {
			fseek(fd, len - 6, SEEK_CUR);
			size -= (len - 6);
		}
		if (size <= 0)break;
	}
}

void vObject::Read_3DS_ObjectHMatrixData(FILE*fd, int size)
{
	unsigned short ID;
	unsigned int len;
	while (1) {
		fread(&ID, sizeof(short), 1, fd);
		fread(&len, sizeof(int), 1, fd);
		if (ID == 0xB002) {
			Read_3DS_ObjectMeshInfo(fd, len-6);
		}
		else {
			fseek(fd, len - 6, SEEK_CUR);
		}
		size -= len;
		if (size <= 0)break;
	}
}

void vObject::Read_3DS_ObjectMeshInfo(FILE*fd, int size)
{
	unsigned short ID;
	unsigned int len,keys;
	_3DS* obj = NULL;
	while (1) {
		fread(&ID, sizeof(short), 1, fd);
		fread(&len, sizeof(int), 1, fd);
		if (ID == 0xB010) {
			obj = Read_3DS_ObjectMaterialName(fd);
		}
		else if (ID == 0xB013) {
			Read_3DS_ObjectPIVOT(fd, obj);
		}
		else if (ID == 0xB020 || ID == 0xB021 || ID == 0xB022) {
			fseek(fd, 10, SEEK_CUR);
			fread(&keys, 4, 1, fd);
			if (ID == 0xB020) Read_3DS_ObjectPositionData(fd, obj, keys);
			if (ID == 0xB021) Read_3DS_ObjectQuaternionData(fd, obj, keys);
			if (ID == 0xB022) Read_3DS_ObjectScailData(fd, obj, keys);
		}
		else {
			fseek(fd, len - 6, SEEK_CUR);	
		}	
		size -= len;
		if (size <= 0)break;
	}
}


_3DS* vObject::Read_3DS_ObjectMaterialName(FILE*fd )
{
	char c[500] = { 0, };
	int i = 0;
	short flag, hierarchy;
	NODE_OBJECT* obj;
	while (1) { 
		fread(&c[i], 1, 1, fd);
		if (c[i++] == '\0')break;
	}
	for (i = 0; i < list.GetSize(); i++) {
		obj = (NODE_OBJECT*)list.Search(i);
		if (obj->strName == (const char*)c)break;
	}
	fread(&flag, 2, 1, fd);
	fread(&flag, 2, 1, fd);
	fread(&hierarchy, 2, 1, fd);
	if (obj) {
		obj->parents = (_3DS*)list.Search(hierarchy - 2);
		if (obj->parents)obj->parents->Child.AddTail(obj);
		else root = obj;
	}
	return (_3DS*)obj;
}

void vObject::Read_3DS_ObjectPIVOT(FILE*fd, _3DS* obj)
{
	dhVector v;
	fread(&v, sizeof(dhVector), 1, fd);
	if(obj)obj->mesh.pivot = v;
}
void vObject::Read_3DS_ObjectPositionData(FILE* fd, _3DS* obj, unsigned int keys)
{
	unsigned int key,i;
	dhVector v;
	for (i = 0; i < keys; i++) {
		fseek(fd,6, SEEK_CUR);
		fread(&v, sizeof(dhVector), 1, fd);
		v = v * -1;
		if(obj)obj->SetT(v);
	}
}

void vObject::Read_3DS_ObjectQuaternionData(FILE* fd, _3DS*obj, unsigned int keys)
{
	unsigned int key, i;
	float q;
	dhVector v;
	for (i = 0; i < keys; i++) {
		fseek(fd, 6, SEEK_CUR);
		fread(&q, sizeof(float), 1, fd);
		fread(&v, sizeof(dhVector), 1, fd);
		v = v.Unit();
		if (obj)obj->SetQuat(q, v);
	}
}

void vObject::Read_3DS_ObjectScailData(FILE*fd , _3DS*obj, unsigned int keys)
{
	unsigned int key, i;
	dhVector v;
	for (i = 0; i < keys; i++) {
		fseek(fd, 6, SEEK_CUR);
		fread(&v, sizeof(dhVector), 1, fd);
		// (obj)obj->SetScail(v.x,v.y,v.z);
	}
}
