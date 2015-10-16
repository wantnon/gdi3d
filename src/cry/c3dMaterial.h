//

//  c3dMaterial.h

//  HelloCpp

//

//  Created by yang chao (wantnon) on 14-1-5.

//

//



#ifndef __HelloCpp__c3dMaterial__

#define __HelloCpp__c3dMaterial__



#include <iostream>

using namespace std;


#include "c3dVector.h"

class Cc3dMaterial

{

public:

    Cc3dMaterial(){

        

        

        m_shininess=2.0;//what is the default value of shininess in opengl?

        

        

        //the default values are ref from <<OpenGL Programming Guide>> sixth edition

        m_ambient.init(0.2, 0.2, 0.2, 1);

        m_diffuse.init(0.8, 0.8, 0.8, 1);

        m_specular.init(0, 0, 0, 1);

       

    }

    virtual~Cc3dMaterial(){

        

    }

   

public:

    Cc3dVector4 m_ambient;

    Cc3dVector4 m_diffuse;

    Cc3dVector4 m_specular;

    float m_shininess;

};

#endif /* defined(__HelloCpp__c3dMaterial__) */
