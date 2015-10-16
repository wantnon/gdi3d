//

//  c3dIDTriangle.cpp

//  HelloCpp

//

//  Created by Yang Chao (wantnon) on 14-1-2.

//

//


#include "StdAfx.h"
#include "c3dIDTriangle.h"

Cc3dIDTriangle::Cc3dIDTriangle(int ID0,int ID1,int ID2){

    initMembers();

    init(ID0, ID1, ID2);

}



void Cc3dIDTriangle::init(int ID0,int ID1,int ID2){

    m_array[0]=ID0;

    m_array[1]=ID1;

    m_array[2]=ID2;

}

short Cc3dIDTriangle::vID(int i)const {

    assert(i>=0&&i<=3);

    return m_array[i];

}

void Cc3dIDTriangle::setVID(int i,short vID){

    assert(i>=0&&i<=3);

    m_array[i]=vID;

}

void Cc3dIDTriangle::initMembers(){

    memset(&m_array, 0, sizeof(m_array));

}



