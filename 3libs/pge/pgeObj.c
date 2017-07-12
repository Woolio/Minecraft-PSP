/*
 * This file is part of "Phoenix Game Engine".
 *
 * Copyright (C) 2008 Phoenix Game Engine
 * Copyright (C) 2008 InsertWittyName <tias_dp@hotmail.com>
 * Copyright (C) 2008 MK2k <pge@mk2k.net>
 *
 * Phoenix Game Engine is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 
 * Phoenix Game Engine is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 
 * You should have received a copy of the GNU General Public License
 * along with Phoenix Game Engine.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <pspgu.h>
#include <string.h>
#include <stdio.h>

#include "pge.h"
#include "pgeObj.h"
#include "pgeGfx.h"
#include "pgeMath.h"

typedef struct pgeObjMtlEntry
{
	float ambient[3];
	float diffuse[3];
	float specular[3];
	float shinyness;
	char *name;
	
} pgeObjMtlEntry;

typedef struct pgeObjMtl
{
	unsigned int nummaterials;
	pgeObjMtlEntry *materials;
	
} pgeObjMtl;

static int strequal(char *a, char *b, char *e, int count)
{
	int c;
	c = 0;
	
	while((c < count) && (a != e))
	{
		 if (a[c] != b[c])
			return 0;
			
		 c++;
     }
	 
     if(a == e)
		return 0;
		
	return 1;
}

static int isdeadspace(char a)
{
	if(a < 33)
		return 1;
		
	return 0;
}

static int iseol(char a)
{
	if(a == 10)
		return 1;
		
	return 0;
}

static unsigned int pgeObjSetColorFromMaterial(pgeObjMtl *material, const char *matname)
{
	int i;
	
	unsigned int color = 0xFFFFFFFF;
	
	for(i = 0;i < material->nummaterials;i++)
	{
		if(strcmp(material->materials[i].name, matname) == 0)
			break;
	}
	
	color = GU_COLOR(material->materials[i].ambient[0], material->materials[i].ambient[1], material->materials[i].ambient[2], 1.0f);
	
	return color;
}

static pgeObjMtl *pgeObjLoadMaterial(const char *matname)
{
	int fd = sceIoOpen(matname, PSP_O_RDONLY, 0777);
	
	if(fd < 0)
		return NULL;

	long filesize;
	
	filesize = sceIoLseek32(fd, 0, PSP_SEEK_END);
	sceIoLseek32(fd, 0, PSP_SEEK_SET);
	
	unsigned char *data = pgeMalloc(filesize);
	
	if(!data)
		return NULL;
	
	sceIoRead(fd, data, filesize);
	
	sceIoClose(fd);
	
	unsigned char *currentpos, *endpos;
	unsigned int position = 0;
	unsigned int numnewmtl = 0;
	unsigned int newmtlpos = 0, kapos = 0, kdpos = 0, kspos = 0, nspos = 0; 
	char readbuffer[512];
	
	currentpos = data;
	endpos = currentpos + filesize;
		
	while(currentpos != endpos)
	{
		position = 0;
		
		while((isdeadspace(*currentpos)) && (currentpos != endpos))
			currentpos++;
			
		while((!iseol(*currentpos)) && (currentpos != endpos) && (position < 512))
		{
			readbuffer[position++] = *currentpos;
			currentpos++;
		}
		
		readbuffer[position] = 0;
		
		if(strequal(readbuffer, "newmtl", &readbuffer[position], 6))
			numnewmtl++;
	}
	
	// Allocate what we need
	
	pgeObjMtl *mtl = pgeMalloc(sizeof(pgeObjMtl));
	
	if(!mtl)
	{
		pgeFree(data);
		return NULL;
	}
	
	mtl->materials = (pgeObjMtlEntry *)pgeMalloc(sizeof(pgeObjMtlEntry) * numnewmtl);
	
	if(!mtl->materials)
	{
		pgeFree(mtl);
		pgeFree(data);
		return NULL;
	}
	
	mtl->nummaterials = numnewmtl;
	
	// Read back through and populate
	
	currentpos = data;
	
	char namebuffer[128];
	
	while(currentpos != endpos)
	{
		position = 0;
		
		while((isdeadspace(*currentpos)) && (currentpos != endpos))
			currentpos++;
			
		while((!iseol(*currentpos)) && (currentpos != endpos) && (position < 512))
		{
			readbuffer[position++] = *currentpos;
			currentpos++;
		}
		
		readbuffer[position] = 0;
		
		if(strequal(readbuffer, "newmtl", &readbuffer[position], 6))
		{
			sscanf(readbuffer, "newmtl %s", namebuffer);
			mtl->materials[newmtlpos].name = pgeMalloc(strlen(namebuffer) + 1);
			
			if(!mtl->materials[newmtlpos].name)
			{
				pgeFree(mtl->materials);
				pgeFree(mtl);
				pgeFree(data);
				return NULL;
			}
			
			strcpy(mtl->materials[newmtlpos].name, namebuffer);
			
			newmtlpos++;
		}
		else if(strequal(readbuffer, "Ka", &readbuffer[position], 2))
		{
			sscanf(readbuffer, "Ka %f %f %f", &mtl->materials[kapos].ambient[0], &mtl->materials[kapos].ambient[1], &mtl->materials[kapos].ambient[2]);
			kapos++;
		}
		else if(strequal(readbuffer, "Kd", &readbuffer[position], 2))
		{
			sscanf(readbuffer, "Kd %f %f %f", &mtl->materials[kdpos].diffuse[0], &mtl->materials[kdpos].diffuse[1], &mtl->materials[kdpos].diffuse[2]);
			kdpos++;
		}
		else if(strequal(readbuffer, "Ks", &readbuffer[position], 2))
		{
			sscanf(readbuffer, "Ks %f %f %f", &mtl->materials[kspos].specular[0], &mtl->materials[kspos].specular[1], &mtl->materials[kspos].specular[2]);
			kspos++;
		}
		else if(strequal(readbuffer, "Ns", &readbuffer[position], 2))
		{
			sscanf(readbuffer, "Ns %f", &mtl->materials[nspos].shinyness);
			nspos++;
		}
	}
		
	return mtl;
}

static pgeObj *pgeObjLoadInternal(unsigned char *buffer, unsigned int size)
{
	typedef struct objVertex
	{
		float x, y, z;
		
	} objVertex;
	
	typedef objVertex objNormal;
	
	typedef struct objTexCoord
	{
		float u, v;
		
	} objTexCoord;
	
	typedef struct objTriangle
	{
		unsigned int vertex[3];
		unsigned int normal[3];
		unsigned int texcoord[3];
		unsigned int color;
		
	} objTriangle;
	
	typedef struct objModel
	{
		unsigned int numVertex, numNormal, numTexCoord, numTriangle, numMaterial;
		objVertex *vertexArray;
		objNormal *normalArray;
		objTexCoord *texCoordArray;
		objTriangle *triangleArray;
	 
	 } objModel;
	 
	 objModel *model = pgeMalloc(sizeof(objModel));
	 
	 pgeObjMtl *mtl = NULL;
	 
	 unsigned char *currentpos, *endpos;
	 unsigned int position = 0;
	 int vertpos = 0, normalpos = 0, texvertpos = 0, facepos = 0;
	 char readbuffer[512];
	 char materialname[128];
	 
	 currentpos = buffer;
	 endpos = buffer + size;
	
	// Seek through file, counting number of entries we are interesting in
	
	while(currentpos != endpos)
	{
		position = 0;
		
		while((isdeadspace(*currentpos)) && (currentpos != endpos))
			currentpos++;
			
		while((!iseol(*currentpos)) && (currentpos != endpos) && (position < 512))
		{
			readbuffer[position++] = *currentpos;
			currentpos++;
		}
		
		readbuffer[position] = 0;
		
		if(strequal(readbuffer, "vn", &readbuffer[position], 2))
			model->numNormal++;
		else if(strequal(readbuffer, "vt", &readbuffer[position], 2))
			model->numTexCoord++;
		else if(strequal(readbuffer, "v", &readbuffer[position], 1))
			model->numVertex++;
		else if(strequal(readbuffer, "f", &readbuffer[position], 1))
			model->numTriangle++;
		else if(strequal(readbuffer, "mtllib", &readbuffer[position], 6))
			model->numMaterial++;
	}
			
	// Allocate what we need
	if(model->numVertex > 0)
	{
		model->vertexArray = pgeMalloc(model->numVertex * sizeof(objVertex));
		
		if(!model->vertexArray)
		{
			pgeFree(model);
			return NULL;
		}
	}
	
	if(model->numNormal > 0)
	{
		model->normalArray = pgeMalloc(model->numNormal * sizeof(objNormal));
		
		if(!model->normalArray)
		{
			pgeFree(model->vertexArray);
			pgeFree(model);
			return NULL;
		}
	}
	
	if(model->numTexCoord > 0)
	{
		model->texCoordArray = pgeMalloc(model->numTexCoord * sizeof(objTexCoord));
		
		if(!model->texCoordArray)
		{
			pgeFree(model->vertexArray);
			pgeFree(model->normalArray);
			pgeFree(model);
			return NULL;
		}
	}
	
	if(model->numTriangle > 0)
	{
		model->triangleArray = pgeMalloc(model->numTriangle * sizeof(objTriangle));
		
		if(!model->triangleArray)
		{
			pgeFree(model->vertexArray);
			pgeFree(model->normalArray);
			pgeFree(model->texCoordArray);
			pgeFree(model);
			return NULL;
		}
	}
	
	// Read from the start of the file again and fill our arrays
	
	currentpos = buffer;
	
	unsigned int currentcolor = 0xFFFFFFFF;
	
	float tempnv = 0.0f;
	
	while(currentpos != endpos)
	{
		position = 0;
		
		while((isdeadspace(*currentpos)) && (currentpos != endpos))
			currentpos++;
			
		while((!iseol(*currentpos)) && (currentpos != endpos) && (position < 512))
		{
			readbuffer[position++] = *currentpos;
			currentpos++;
		}
		
		readbuffer[position] = 0;
		
		if(strequal(readbuffer, "vn", &readbuffer[position], 2))
		{
			sscanf(readbuffer, "vn %f %f %f", &model->normalArray[normalpos].x, &model->normalArray[normalpos].y, &model->normalArray[normalpos].z);
			normalpos++;
		}
		else if(strequal(readbuffer, "vt", &readbuffer[position], 2))
		{
			sscanf(readbuffer, "vt %f %f", &model->texCoordArray[texvertpos].u, &model->texCoordArray[texvertpos].v);
			texvertpos++;
		}
		else if(strequal(readbuffer, "v", &readbuffer[position], 1))
		{
			sscanf(readbuffer, "v %f %f %f", &model->vertexArray[vertpos].x, &model->vertexArray[vertpos].y, &model->vertexArray[vertpos].z);
			vertpos++;
		}
		else if(strequal(readbuffer, "f", &readbuffer[position], 1))
		{
			if(model->numMaterial > 0)
				model->triangleArray[facepos].color = currentcolor;
			
			if(model->numTexCoord > 0 && model->numNormal > 0)
			{
				sscanf(readbuffer, "f %d/%d/%d %d/%d/%d %d/%d/%d", &model->triangleArray[facepos].vertex[0], &model->triangleArray[facepos].texcoord[0], &model->triangleArray[facepos].normal[0], &model->triangleArray[facepos].vertex[1], &model->triangleArray[facepos].texcoord[1], &model->triangleArray[facepos].normal[1], &model->triangleArray[facepos].vertex[2], &model->triangleArray[facepos].texcoord[2], &model->triangleArray[facepos].normal[2]);
				facepos++;
			}
			else if(model->numNormal > 0)
			{
				sscanf(readbuffer, "f %d//%d %d//%d %d//%d", &model->triangleArray[facepos].vertex[0], &model->triangleArray[facepos].normal[0], &model->triangleArray[facepos].vertex[1], &model->triangleArray[facepos].normal[1], &model->triangleArray[facepos].vertex[2], &model->triangleArray[facepos].normal[2]);
				facepos++;
			}
			else if(model->numTexCoord > 0)
			{
				sscanf(readbuffer, "f %d/%d %d/%d %d/%d", &model->triangleArray[facepos].vertex[0], &model->triangleArray[facepos].texcoord[0], &model->triangleArray[facepos].vertex[1], &model->triangleArray[facepos].texcoord[1], &model->triangleArray[facepos].vertex[2], &model->triangleArray[facepos].texcoord[2]);
				facepos++;
			}
			else
			{
				sscanf(readbuffer, "f %d %d %d", &model->triangleArray[facepos].vertex[0], &model->triangleArray[facepos].vertex[1], &model->triangleArray[facepos].vertex[2]);
				facepos++;
			}
		}
		else if(strequal(readbuffer, "mtllib", &readbuffer[position], 6))
		{
			sscanf(readbuffer, "mtllib %s", materialname);
			mtl = pgeObjLoadMaterial(materialname);
			
			if(!mtl)
			{
				pgeFree(model->vertexArray);
				pgeFree(model->normalArray);
				pgeFree(model->texCoordArray);
				pgeFree(model);
				return NULL;
			}
		}
		else if(strequal(readbuffer, "usemtl", &readbuffer[position], 6))
		{
			sscanf(readbuffer, "usemtl %s", materialname);
			currentcolor = pgeObjSetColorFromMaterial(mtl, materialname);
		}
	}
	
	pgeObj *obj = (pgeObj *)pgeMalloc(sizeof(pgeObj));
	
	if(!obj)
	{
		pgeFree(model->vertexArray);
		pgeFree(model->normalArray);
		pgeFree(model->texCoordArray);
		pgeFree(model->triangleArray);
		pgeFree(model);
		return NULL;
	}
	
	memset(obj, 0, sizeof(obj));
	
	// Arrange the data into our vert array.
	obj->numvertices = model->numTriangle * 3;
	
	int i = 0;
	int f = 0;
	int v = 0;
	
	if(model->numTexCoord > 0 && model->numNormal > 0)
	{
		if(model->numMaterial == 0)
		{
			obj->vertices =  (pgeVertTNV *)pgeMalloc(sizeof(pgeVertTNV) * model->numTriangle * 3);
			pgeVertTNV *ptr = (pgeVertTNV *)obj->vertices;
			obj->drawflag = GU_TEXTURE_32BITF|GU_NORMAL_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D;
			
			for(i = 0; i < model->numTriangle;i++)
			{
				for(f = 0; f < 3;f++)
				{
					ptr[v].u = model->texCoordArray[model->triangleArray[i].texcoord[f]-1].u;
					ptr[v].v = 1.0f - model->texCoordArray[model->triangleArray[i].texcoord[f]-1].v;
		
					ptr[v].x = model->vertexArray[model->triangleArray[i].vertex[f]-1].x;
					ptr[v].y = model->vertexArray[model->triangleArray[i].vertex[f]-1].y;
					ptr[v].z = model->vertexArray[model->triangleArray[i].vertex[f]-1].z;
					
					tempnv = 1.0f/(pgeMathSqrt(ptr[v].x*ptr[v].x + ptr[v].y*ptr[v].y + ptr[v].z*ptr[v].z));
					ptr[v].nx = ptr[v].x * tempnv;
					ptr[v].ny = ptr[v].y * tempnv;
					ptr[v].nz = ptr[v].z * tempnv;
					
					v++;
				}
			}
		}
		else
		{
			obj->vertices =  (pgeVertTCNV *)pgeMalloc(sizeof(pgeVertTCNV) * model->numTriangle * 3);
			pgeVertTCNV *ptr = (pgeVertTCNV *)obj->vertices;
			obj->drawflag = GU_COLOR_8888|GU_TEXTURE_32BITF|GU_NORMAL_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D;
			
			for(i = 0; i < model->numTriangle;i++)
			{
				for(f = 0; f < 3;f++)
				{
					ptr[v].u = model->texCoordArray[model->triangleArray[i].texcoord[f]-1].u;
					ptr[v].v = 1.0f - model->texCoordArray[model->triangleArray[i].texcoord[f]-1].v;
					
					ptr[v].color = model->triangleArray[i].color;
		
					ptr[v].x = model->vertexArray[model->triangleArray[i].vertex[f]-1].x;
					ptr[v].y = model->vertexArray[model->triangleArray[i].vertex[f]-1].y;
					ptr[v].z = model->vertexArray[model->triangleArray[i].vertex[f]-1].z;
					
					tempnv = 1.0f/(pgeMathSqrt(ptr[v].x*ptr[v].x + ptr[v].y*ptr[v].y + ptr[v].z*ptr[v].z));
					ptr[v].nx = ptr[v].x * tempnv;
					ptr[v].ny = ptr[v].y * tempnv;
					ptr[v].nz = ptr[v].z * tempnv;
					
					v++;
				}
			}
		}
		
	}
	else if(model->numTexCoord > 0)
	{
		if(model->numMaterial == 0)
		{
			obj->vertices = (pgeVertTV *)pgeMalloc(sizeof(pgeVertTV) * model->numTriangle * 3);
			pgeVertTV *ptr = (pgeVertTV *)obj->vertices;
			obj->drawflag = GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D;
		
			for(i = 0; i < model->numTriangle;i++)
			{
				for(f = 0; f < 3;f++)
				{
					ptr[v].u = model->texCoordArray[model->triangleArray[i].texcoord[f]-1].u;
					ptr[v].v = 1.0f - model->texCoordArray[model->triangleArray[i].texcoord[f]-1].v;
		
					ptr[v].x = model->vertexArray[model->triangleArray[i].vertex[f]-1].x;
					ptr[v].y = model->vertexArray[model->triangleArray[i].vertex[f]-1].y;
					ptr[v].z = model->vertexArray[model->triangleArray[i].vertex[f]-1].z;
					
					v++;
				}
			}
		}
		else
		{
			obj->vertices = (pgeVertTCV *)pgeMalloc(sizeof(pgeVertTCV) * model->numTriangle * 3);
			pgeVertTCV *ptr = (pgeVertTCV *)obj->vertices;
			obj->drawflag = GU_COLOR_8888|GU_TEXTURE_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D;
		
			for(i = 0; i < model->numTriangle;i++)
			{
				for(f = 0; f < 3;f++)
				{
					ptr[v].u = model->texCoordArray[model->triangleArray[i].texcoord[f]-1].u;
					ptr[v].v = 1.0f - model->texCoordArray[model->triangleArray[i].texcoord[f]-1].v;
					
					ptr[v].color = model->triangleArray[i].color;
		
					ptr[v].x = model->vertexArray[model->triangleArray[i].vertex[f]-1].x;
					ptr[v].y = model->vertexArray[model->triangleArray[i].vertex[f]-1].y;
					ptr[v].z = model->vertexArray[model->triangleArray[i].vertex[f]-1].z;
					
					v++;
				}
			}
		}
	}
	else if(model->numNormal > 0)
	{
		if(model->numMaterial == 0)
		{
			obj->vertices = (pgeVertNV *)pgeMalloc(sizeof(pgeVertNV) * model->numTriangle * 3);
			pgeVertNV *ptr = (pgeVertNV *)obj->vertices;
			obj->drawflag = GU_NORMAL_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D;
		
			for(i = 0; i < model->numTriangle;i++)
			{
				for(f = 0; f < 3;f++)
				{
					ptr[v].x = model->vertexArray[model->triangleArray[i].vertex[f]-1].x;
					ptr[v].y = model->vertexArray[model->triangleArray[i].vertex[f]-1].y;
					ptr[v].z = model->vertexArray[model->triangleArray[i].vertex[f]-1].z;
					
					tempnv = 1.0f/(pgeMathSqrt(ptr[v].x*ptr[v].x + ptr[v].y*ptr[v].y + ptr[v].z*ptr[v].z));
					ptr[v].nx = ptr[v].x * tempnv;
					ptr[v].ny = ptr[v].y * tempnv;
					ptr[v].nz = ptr[v].z * tempnv;

					v++;
				}
			}
		}
		else
		{
			obj->vertices = (pgeVertCNV *)pgeMalloc(sizeof(pgeVertCNV) * model->numTriangle * 3);
			pgeVertCNV *ptr = (pgeVertCNV *)obj->vertices;
			obj->drawflag = GU_COLOR_8888|GU_NORMAL_32BITF|GU_VERTEX_32BITF|GU_TRANSFORM_3D;
		
			for(i = 0; i < model->numTriangle;i++)
			{
				for(f = 0; f < 3;f++)
				{
					ptr[v].color = model->triangleArray[i].color;
		
					ptr[v].x = model->vertexArray[model->triangleArray[i].vertex[f]-1].x;
					ptr[v].y = model->vertexArray[model->triangleArray[i].vertex[f]-1].y;
					ptr[v].z = model->vertexArray[model->triangleArray[i].vertex[f]-1].z;

					tempnv = 1.0f/(pgeMathSqrt(ptr[v].x*ptr[v].x + ptr[v].y*ptr[v].y + ptr[v].z*ptr[v].z));
					ptr[v].nx = ptr[v].x * tempnv;
					ptr[v].ny = ptr[v].y * tempnv;
					ptr[v].nz = ptr[v].z * tempnv;

					v++;
				}
			}
		}
	}
	else
	{
		if(model->numMaterial == 0)
		{
			obj->vertices = (pgeVertV *)pgeMalloc(sizeof(pgeVertV) * model->numTriangle * 3);
			pgeVertV *ptr = (pgeVertV *)obj->vertices;
			obj->drawflag = GU_VERTEX_32BITF|GU_TRANSFORM_3D;
		
			for(i = 0; i < model->numTriangle;i++)
			{
				for(f = 0; f < 3;f++)
				{
					ptr[v].x = model->vertexArray[model->triangleArray[i].vertex[f]-1].x;
					ptr[v].y = model->vertexArray[model->triangleArray[i].vertex[f]-1].y;
					ptr[v].z = model->vertexArray[model->triangleArray[i].vertex[f]-1].z;
					
					v++;
				}
			}
		}
		else
		{
			obj->vertices = (pgeVertCV *)pgeMalloc(sizeof(pgeVertCV) * model->numTriangle * 3);
			pgeVertCV *ptr = (pgeVertCV *)obj->vertices;
			obj->drawflag = GU_COLOR_8888|GU_VERTEX_32BITF|GU_TRANSFORM_3D;
		
			for(i = 0; i < model->numTriangle;i++)
			{
				for(f = 0; f < 3;f++)
				{
					ptr[v].color = model->triangleArray[i].color;
					
					ptr[v].x = model->vertexArray[model->triangleArray[i].vertex[f]-1].x;
					ptr[v].y = model->vertexArray[model->triangleArray[i].vertex[f]-1].y;
					ptr[v].z = model->vertexArray[model->triangleArray[i].vertex[f]-1].z;
					
					v++;
				}
			}
		}
	}
	
	// Tidy up
	if(model->vertexArray)
		pgeFree(model->vertexArray);
		
	if(model->normalArray)
		pgeFree(model->normalArray);
		
	if(model->texCoordArray)
		pgeFree(model->texCoordArray);
		
	if(model->triangleArray)
		pgeFree(model->triangleArray);
		
	if(model)
		pgeFree(model);
		
	if(mtl)
	{
		for(i = 0;i < mtl->nummaterials;i++)
		{
			if(mtl->materials[i].name)
				pgeFree(mtl->materials[i].name);
				
			if(mtl->materials)
				pgeFree(mtl->materials);
		}
		
		pgeFree(mtl);
	}
		
	sceKernelDcacheWritebackInvalidateAll();
	
	return obj;
}

pgeObj *pgeObjLoad(const char *filename)
{	
	int fd = sceIoOpen(filename, PSP_O_RDONLY, 0777);
	
	if(fd < 0)
		return NULL;

	long filesize;
	
	filesize = sceIoLseek32(fd, 0, PSP_SEEK_END);
	sceIoLseek32(fd, 0, PSP_SEEK_SET);
	
	unsigned char *data = pgeMalloc(filesize);
	
	if(!data)
		return NULL;
	
	sceIoRead(fd, data, filesize);
	
	sceIoClose(fd);
	
	pgeObj *obj = pgeObjLoadInternal(data, filesize);
	
	if(data)
		pgeFree(data);
	
	return obj;
}

pgeObj *pgeObjLoadMemory(unsigned char *buffer, unsigned int size)
{	
	return(pgeObjLoadInternal(buffer, size));
}

void pgeObjDestroy(pgeObj *obj)
{
	if(obj->vertices)
		pgeFree(obj->vertices);
		
	if(obj)
		pgeFree(obj);
}
