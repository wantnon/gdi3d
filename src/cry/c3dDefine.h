//
//  c3dDefine.h
//  HelloCpp
//
//  Created by Yang Chao (wantnon) on 14-1-2.
//
//

#ifndef __HelloCpp__c3dDefine__
#define __HelloCpp__c3dDefine__

#include <iostream>
#include <string>
using namespace std;

#define cc3dv4(x,y,z,w) Cc3dVector4(x,y,z,w)
enum Ec3dMatStackName{
    ec3dModelViewStack = 0,
    ec3dProjectionStack,
    ec3dTextureStack,
};

enum Ec3dProjectionMode{
    ec3dOrthographicMode = 0,
    ec3dPerspectiveMode,
};

const float M_PI=3.1415926;
const float M_INF=0.5*(numeric_limits<float>::max)();
const float M_INF_int=0.5*(numeric_limits<int>::max)();
#define MAX_WCSTR_LEN 512
#endif /* defined(__HelloCpp__c3dDefine__) */