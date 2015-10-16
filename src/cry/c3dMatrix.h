//
//  c3dMatrix.h
//  HelloCpp
//
//  Created by wantnon (yang chao) on 14-1-1.
//
//

#ifndef __HelloCpp__c3dMatrix__
#define __HelloCpp__c3dMatrix__

#include <iostream>
#include <vector>
using namespace std;
#include "assert.h"
#include "c3dVector.h"
#include "c3dDefine.h"
#include "c3dLog.h"
class Cc3dMatrix4 
{
public:
    Cc3dMatrix4(){
        initMembers();
    }
    Cc3dMatrix4(const float array[16]);
    Cc3dMatrix4(float a0,float a1,float a2,float a3,//col 1
                         float a4,float a5,float a6,float a7,//col2
                         float a8,float a9,float a10,float a11,//col3
                         float a12,float a13,float a14,float a15//col4
    );
    ~Cc3dMatrix4(){}
    void init(const float array[16]);
    void init(float a0,float a1,float a2,float a3,//col 1
                    float a4,float a5,float a6,float a7,//col2
                    float a8,float a9,float a10,float a11,//col3
                    float a12,float a13,float a14,float a15//col4
    );
    const float* getArray()const{return  m_array;}
    float getAt(int index)const;
    void setAt(int index,float value);
    int getArrayLen()const{return 16;}
    vector<float> convertToVector()const;
    Cc3dMatrix4 operator*(const Cc3dMatrix4&mat)const;
    Cc3dVector4 operator*(const Cc3dVector4&v)const;
	Cc3dMatrix4 operator*(float k)const;
	Cc3dMatrix4 operator+(const Cc3dMatrix4&mat)const;
    void print();
private:
    void initMembers();
private:
    float m_array[16];
  
    
};
#endif /* defined(__HelloCpp__c3dMatrix__) */