//
//  c3dMatrix.cpp
//  HelloCpp
//
//  Created by wantnon (yang chao) on 14-1-1.
//
//
#include "StdAfx.h"
#include "c3dMatrix.h"

Cc3dMatrix4::Cc3dMatrix4(const float array[16]){
    initMembers();
    init(array);
}
Cc3dMatrix4::Cc3dMatrix4(float a0,float a1,float a2,float a3,//col 1
            float a4,float a5,float a6,float a7,//col2
            float a8,float a9,float a10,float a11,//col3
            float a12,float a13,float a14,float a15//col4
){
    initMembers();
    init(a0,a1,a2,a3,
         a4,a5,a6,a7,
         a8,a9,a10,a11,
         a12,a13,a14,a15);
}
void Cc3dMatrix4::init(const float array[16]){
    for(int i=0;i<16;i++){
        m_array[i]=array[i];
    }
}
void Cc3dMatrix4::initMembers(){
    memset(&m_array, 0, sizeof(m_array));
}
void Cc3dMatrix4::init(float a0,float a1,float a2,float a3,//col 1
          float a4,float a5,float a6,float a7,//col2
          float a8,float a9,float a10,float a11,//col3
          float a12,float a13,float a14,float a15//col4
){
    m_array[0]=a0;
    m_array[1]=a1;
    m_array[2]=a2;
    m_array[3]=a3;
    m_array[4]=a4;
    m_array[5]=a5;
    m_array[6]=a6;
    m_array[7]=a7;
    m_array[8]=a8;
    m_array[9]=a9;
    m_array[10]=a10;
    m_array[11]=a11;
    m_array[12]=a12;
    m_array[13]=a13;
    m_array[14]=a14;
    m_array[15]=a15;
}

float Cc3dMatrix4::getAt(int index)const{
    assert(index>=0&&index<=15);
    return m_array[index];
}
void Cc3dMatrix4::setAt(int index,float value){
    assert(index>=0&&index<=15);
    m_array[index]=value;
}
vector<float> Cc3dMatrix4::convertToVector()const{
    vector<float> vec(m_array,m_array+16);
    return vec;
}
Cc3dMatrix4 Cc3dMatrix4::operator+(const Cc3dMatrix4&mat)const{
	const float *a=this->getArray();
    const float *b=mat.getArray();
    float r[16];//result
	for(int i=0;i<16;i++){
		r[i]=a[i]+b[i];
	}
	return Cc3dMatrix4(r);
}
Cc3dMatrix4 Cc3dMatrix4::operator*(const Cc3dMatrix4&mat)const{
    const float *a=this->getArray();
    const float *b=mat.getArray();
    float r[16];//result
    r[0]=b[0]*a[0]+b[1]*a[4]+b[2]*a[8]+b[3]*a[12];
    r[1]=b[0]*a[1]+b[1]*a[5]+b[2]*a[9]+b[3]*a[13];
    r[2]=b[0]*a[2]+b[1]*a[6]+b[2]*a[10]+b[3]*a[14];
    r[3]=b[0]*a[3]+b[1]*a[7]+b[2]*a[11]+b[3]*a[15];
    
    r[4]=b[4]*a[0]+b[5]*a[4]+b[6]*a[8]+b[7]*a[12];
    r[5]=b[4]*a[1]+b[5]*a[5]+b[6]*a[9]+b[7]*a[13];
    r[6]=b[4]*a[2]+b[5]*a[6]+b[6]*a[10]+b[7]*a[14];
    r[7]=b[4]*a[3]+b[5]*a[7]+b[6]*a[11]+b[7]*a[15];
    
    r[8]=b[8]*a[0]+b[9]*a[4]+b[10]*a[8]+b[11]*a[12];
    r[9]=b[8]*a[1]+b[9]*a[5]+b[10]*a[9]+b[11]*a[13];
    r[10]=b[8]*a[2]+b[9]*a[6]+b[10]*a[10]+b[11]*a[14];
    r[11]=b[8]*a[3]+b[9]*a[7]+b[10]*a[11]+b[11]*a[15];
    
    r[12]=b[12]*a[0]+b[13]*a[4]+b[14]*a[8]+b[15]*a[12];
    r[13]=b[12]*a[1]+b[13]*a[5]+b[14]*a[9]+b[15]*a[13];
    r[14]=b[12]*a[2]+b[13]*a[6]+b[14]*a[10]+b[15]*a[14];
    r[15]=b[12]*a[3]+b[13]*a[7]+b[14]*a[11]+b[15]*a[15];
    return Cc3dMatrix4(r);
}
Cc3dVector4 Cc3dMatrix4::operator*(const Cc3dVector4&v)const{
    const float *m=this->getArray();
    const float *vin=v.getArray();
    float vout[4];
    vout[0]=vin[0]*m[0]+vin[1]*m[4]+vin[2]*m[8]+vin[3]*m[12];
    vout[1]=vin[0]*m[1]+vin[1]*m[5]+vin[2]*m[9]+vin[3]*m[13];
    vout[2]=vin[0]*m[2]+vin[1]*m[6]+vin[2]*m[10]+vin[3]*m[14];
    vout[3]=vin[0]*m[3]+vin[1]*m[7]+vin[2]*m[11]+vin[3]*m[15];
    return Cc3dVector4(vout);
}

Cc3dMatrix4 Cc3dMatrix4::operator*(float k)const{
    const float *m=this->getArray();
	float r[16];
	for(int i=0;i<16;i++){
		r[i]=m[i]*k;
	}
    return Cc3dMatrix4(r);
}

void Cc3dMatrix4::print(){
    c3dLog("c3dMatrix: %f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
           m_array[0],m_array[1],m_array[2],m_array[3],
           m_array[4],m_array[5],m_array[6],m_array[7],
           m_array[8],m_array[9],m_array[10],m_array[11],
           m_array[12],m_array[13],m_array[14],m_array[15]);
}
