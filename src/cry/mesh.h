


#ifndef __HelloCpp__mesh__

#define __HelloCpp__mesh__



#include <iostream>

#include "c3dVector.h"

#include "c3dIDTriangle.h"
#include "simpleClasses.h"

class Cmesh

{

public:

    Cmesh(){};

    virtual~Cmesh(){};

	void resizeVerts(int vertexCount);
	void resizeIDtris(int IDtriCount){
		m_IDtriList.resize(IDtriCount);

	}
	void clearAll(){
		resizeVerts(0);
		resizeIDtris(0);
	}
	Cvert getVert(int vertIndex);
	void setVert(int vertIndex,const Cvert&v);
	void pushBackVert(const Cvert&v);
public:
    vector<Cc3dVector4> m_positionList;
	vector<int> m_textureIDList;
	vector<int> m_materialIDList;
    vector<Cc3dVector2> m_texCoordList;
    vector<Cc3dVector4> m_normalList;
    vector<Cc3dVector4> m_colorList;
	vector<Cc3dVector4> m_colorAddList;
	vector<bool> m_isCullBackList;
public:
    vector<Cc3dIDTriangle> m_IDtriList;


};
#endif /* defined(__HelloCpp__mesh__) */