//

//  c3dCamera.h

//  HelloCpp

//

//  Created by Yang Chao (wantnon) on 14-1-7.

//

//



#ifndef __HelloCpp__c3dCamera__

#define __HelloCpp__c3dCamera__



#include <iostream>

using namespace std;



#include "c3dVector.h"

#include "c3dMatrix.h"

#include "c3dGLMath.h"


#include "c3dRange.h"
class Cc3dSize{
public:
	float m_width;
	float m_height;
	Cc3dSize(){}
	Cc3dSize(float width,float height){m_width=width;m_height=height;}
	virtual ~Cc3dSize(){}
	float getWidth()const{return m_width;}
	float getHeight()const{return m_height;}
};
class Cc3dCamera

{

public:

    Cc3dCamera(float w,float h){


        m_fovy=60;

        m_aspect=w/h;

        m_zNear=0.5;

        m_zFar=150;//1500;

       

        m_range.init(-w/2, -w/2+w, -h/2, -h/2+h,

                     1024,//yeah, better to use negative value

                     1024
					 );

        m_projectionMode=ec3dPerspectiveMode;





    }

    virtual ~Cc3dCamera(){

    

    }





    Cc3dVector4 getEyePos()const;

    Cc3dVector4 getCenter()const;

    Cc3dVector4 getUp()const;

    void setEyePos(const Cc3dVector4&eyePos);

    void setCenter(const Cc3dVector4&center);

    void setUp(const Cc3dVector4&up);

    float getFovy()const {return m_fovy;}

    float getAspect()const {return m_aspect;}

    float getzNear()const {return m_zNear;}

    float getzFar()const {return m_zFar;}

    void setFovy(float fovy){m_fovy=fovy;}

    void setAspect(float aspect){m_aspect=aspect;}

    void setzNear(float zNear){m_zNear=zNear;}

    void setzFar(float zFar){m_zFar=zFar;}

    Cc3dRange getRange()const {return m_range;}

    void setRange(const Cc3dRange&range){m_range=range;}

    Cc3dMatrix4 calculateViewMat()const;

    Cc3dMatrix4 calculateViewMatInverse()const;

    Cc3dMatrix4 calculateProjectionMat()const;

    Ec3dProjectionMode getProjectionMode()const{return m_projectionMode;}

    void setProjectionMode(Ec3dProjectionMode projectionMode){m_projectionMode=projectionMode;}


    void printProjectionMode()const;
	void rotate(const Cc3dVector4&axis,float cosA,float sinA);
protected:

    //projection mode type

    Ec3dProjectionMode m_projectionMode;

    //perspective projection mode params

    float m_fovy;

    float m_aspect;

    float m_zNear;

    float m_zFar;

    //Ortho projection mode params

    Cc3dRange m_range;//in the camera space

protected:
	Cc3dVector4 m_eyePos;
	Cc3dVector4 m_center;
	Cc3dVector4 m_up;


    

    



};

#endif /* defined(__HelloCpp__c3dCamera__) */
