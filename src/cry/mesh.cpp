

#include "StdAfx.h"
#include "mesh.h"
 


Cvert Cmesh::getVert(int vertIndex){
	assert(vertIndex<(int)m_positionList.size());
	Cvert v;
	v.m_pos=m_positionList[vertIndex];
	v.m_color=m_colorList[vertIndex];
	v.m_norm=m_normalList[vertIndex];
	v.m_texCoord=m_texCoordList[vertIndex];
	v.m_textureID=m_textureIDList[vertIndex];
	v.m_materialID=m_materialIDList[vertIndex];
	v.m_isCullBack=m_isCullBackList[vertIndex];
	v.m_colorAdd=m_colorAddList[vertIndex];
	return v;
}
void Cmesh::setVert(int vertIndex,const Cvert&v){
	assert(vertIndex<(int)m_positionList.size());
	m_positionList[vertIndex]=v.m_pos;
	m_colorList[vertIndex]=v.m_color;
	m_normalList[vertIndex]=v.m_norm;
	m_texCoordList[vertIndex]=v.m_texCoord;
	m_textureIDList[vertIndex]=v.m_textureID;
	m_materialIDList[vertIndex]=v.m_materialID;
	m_isCullBackList[vertIndex]=v.m_isCullBack;
	m_colorAddList[vertIndex]=v.m_colorAdd;
}
void Cmesh::pushBackVert(const Cvert&v){
	m_positionList.push_back(v.m_pos);
	m_colorList.push_back(v.m_color);
	m_normalList.push_back(v.m_norm);
	m_texCoordList.push_back(v.m_texCoord);
	m_textureIDList.push_back(v.m_textureID);
	m_materialIDList.push_back(v.m_materialID);
	m_isCullBackList.push_back(v.m_isCullBack);
	m_colorAddList.push_back(v.m_colorAdd);

}


void Cmesh::resizeVerts(int vertexCount){
	m_positionList.resize(vertexCount);
	m_textureIDList.resize(vertexCount);
	m_materialIDList.resize(vertexCount);
	m_texCoordList.resize(vertexCount);
	m_normalList.resize(vertexCount);
	m_colorList.resize(vertexCount);
	m_isCullBackList.resize(vertexCount);
	m_colorAddList.resize(vertexCount);

}