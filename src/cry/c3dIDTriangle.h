//

//  c3dIDTriangle.h

//  HelloCpp

//

//  Created by Yang Chao (wantnon) on 14-1-2.

//

//



#ifndef __HelloCpp__c3dIDTriangle__

#define __HelloCpp__c3dIDTriangle__



#include <iostream>

using namespace std;

#include "c3dDefine.h"

#include "assert.h"

#include "c3dLog.h"

class Cc3dIDTriangle 

{

public:

    Cc3dIDTriangle(){

        initMembers();

    }

    ~Cc3dIDTriangle(){

        

    }

    Cc3dIDTriangle(int ID0,int ID1,int ID2);

    const short*getArray()const {return m_array;}

    void init(int ID0,int ID1,int ID2);

    short vID(int i)const;

    void setVID(int i,short vID);

protected:

    void initMembers();

protected:

    short m_array[3];

    

};

#endif /* defined(__HelloCpp__c3dIDTriangle__) */
