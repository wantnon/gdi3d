#ifndef _simpleClasses_H_
#define _simpleClasses_H_
#include "c3dVector.h"
class Cvert{
public:
	Cvert(){
		m_textureID=0;
		m_materialID=0;
		m_isCullBack=true;
		
	}
	~Cvert(){}
public:
	Cc3dVector4 m_pos;
	Cc3dVector4 m_color;
	Cc3dVector4 m_colorAdd;
	Cc3dVector4 m_norm;
	Cc3dVector2 m_texCoord;
	int m_textureID;
	int m_materialID;
	bool m_isCullBack;
};
class Ctri{
public:
	Ctri(){}
	~Ctri(){}
public:
	Cvert m_v0;
	Cvert m_v1;
	Cvert m_v2;
};
class Cfrag{
public:
	Cc3dVector4 m_pos;
	Cc3dVector4 m_color;
	Cfrag(){}
	~Cfrag(){}
};
class CearlyZOutput{
public:
	float m_x,m_y,m_z;
	float m_A,m_B;
	CearlyZOutput(){}
	~CearlyZOutput(){}
};




#endif