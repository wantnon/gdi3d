//
//  c3dVector.cpp
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-1-1.
//
//
#include "StdAfx.h"
#include "c3dVector.h"
//--------Cc3dVector2
void Cc3dVector2::initMembers(){
    memset(&m_array, 0, sizeof(m_array));
}
void Cc3dVector2::init(float a0,float a1){
    m_array[0]=a0;
    m_array[1]=a1;
}
Cc3dVector2::Cc3dVector2(float a0,float a1){
    initMembers();
    init(a0,a1);
}
Cc3dVector2::Cc3dVector2(const float array[2]){
    initMembers();
    init(array);
}
vector<float> Cc3dVector2::convertToVector()const{
    vector<float> vec(m_array,m_array+2);
    return vec;
}
Cc3dVector2 Cc3dVector2::operator +(const Cc3dVector2&right)const {
    Cc3dVector2 rs(this->x()+right.x(),this->y()+right.y());
    return rs;
}
Cc3dVector2 Cc3dVector2::operator -(const Cc3dVector2&right)const {
    Cc3dVector2 rs(this->x()-right.x(),this->y()-right.y());
    return rs;
}
Cc3dVector2 Cc3dVector2::operator *(float k)const {
    Cc3dVector2 rs(this->x()*k,this->y()*k);
    return rs;
}
void Cc3dVector2::init(const float array[2]){
    for(int i=0;i<2;i++){
        m_array[i]=array[i];
    }
}
void Cc3dVector2::print()const {
    C3DLOG("Cc3dVector2: %f,%f",m_array[0],m_array[1]);
}
//--------Cc3dVector4
void Cc3dVector4::init(float a0,float a1,float a2,float a3){
    m_array[0]=a0;
    m_array[1]=a1;
    m_array[2]=a2;
    m_array[3]=a3;
    
}
Cc3dVector4::Cc3dVector4(float a0,float a1,float a2,float a3){
    initMembers();
    init(a0,a1,a2,a3);
}
Cc3dVector4::Cc3dVector4(const float array[4]){
    initMembers();
    init(array);
}

float Cc3dVector4::getAt(int index)const {
    assert(index>=0&&index<=3);
    return m_array[index];
}
vector<float> Cc3dVector4::convertToVector()const{
    vector<float> vec(m_array,m_array+4);
    return vec;
}
Cc3dVector4 Cc3dVector4::operator +(const Cc3dVector4&right)const {
    Cc3dVector4 rs(this->x()+right.x(),this->y()+right.y(),this->z()+right.z(),this->w()+right.w());
    return rs;
}
Cc3dVector4 Cc3dVector4::operator -(const Cc3dVector4&right)const {
    Cc3dVector4 rs(this->x()-right.x(),this->y()-right.y(),this->z()-right.z(),this->w()-right.w());
    return rs;
}
Cc3dVector4 Cc3dVector4::operator *(float k)const {
    Cc3dVector4 rs(this->x()*k,this->y()*k,this->z()*k,this->w()*k);
    return rs;
}
void Cc3dVector4::init(const float array[4]){
    for(int i=0;i<4;i++){
        m_array[i]=array[i];
    }
}
void Cc3dVector4::print()const {
    C3DLOG("Cc3dVector4: %f,%f,%f,%f",m_array[0],m_array[1],m_array[2],m_array[3]);
}