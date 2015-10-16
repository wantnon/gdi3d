//

//  c3dCamera.cpp

//  HelloCpp

//

//  Created by Yang Chao (wantnon) on 14-1-7.

//

//


#include "StdAfx.h"
#include "c3dCamera.h"



Cc3dVector4 Cc3dCamera::getEyePos()const{
	return m_eyePos;
}

Cc3dVector4 Cc3dCamera::getCenter()const{

	return m_center;

}

Cc3dVector4 Cc3dCamera::getUp()const{


	return m_up;

}

void Cc3dCamera::setEyePos(const Cc3dVector4&eyePos){
	m_eyePos=eyePos;


    

}

void Cc3dCamera::setCenter(const Cc3dVector4&center){
	m_center=center;


    

}

void Cc3dCamera::setUp(const Cc3dVector4&up){
	m_up=up;




}



Cc3dMatrix4 Cc3dCamera::calculateViewMat()const {


    Cc3dMatrix4 ret;



    ret=::calculateViewMatrix(getEyePos(), getCenter(), getUp());


    return ret;

    

};

Cc3dMatrix4 Cc3dCamera::calculateViewMatInverse()const {

    Cc3dMatrix4 ret;


        ret=::calculateViewMatrixInverse(getEyePos(), getCenter(), getUp());

 

    return ret;

}

Cc3dMatrix4 Cc3dCamera::calculateProjectionMat()const {

    Cc3dMatrix4 projectionMat;

    switch (m_projectionMode) {

        case ec3dPerspectiveMode:

            projectionMat=::calculatePerspectiveProjectionMatrix(m_fovy, m_aspect, m_zNear, m_zFar);

            break;

        case ec3dOrthographicMode:

            projectionMat=::calculateOrthoProjectionMatrix(m_range.getMinX(), m_range.getMaxX(), m_range.getMinY(), m_range.getMaxY(), m_range.getMinZ(), m_range.getMaxZ());

            break;

        default:

            assert(false);

            break;

    }

    return projectionMat;

}


void Cc3dCamera::printProjectionMode()const{

    if(m_projectionMode==ec3dPerspectiveMode){

        C3DLOG("projectionMode: perspectiveMode");

    }else if(m_projectionMode==ec3dOrthographicMode){

        C3DLOG("projectionMode: orthographic");

    }else{

        assert(false);

    }

}

void Cc3dCamera::rotate(const Cc3dVector4&axis,float cosA,float sinA){
	Cc3dMatrix4 viewMat=calculateViewMat();
	Cc3dMatrix4 modelMatOfCamera=inverse(viewMat);
	Cc3dMatrix4 rotMat=calculateRotationMatrix(axis,cosA,sinA);
	Cc3dMatrix4 newModelMatOfCamera=modelMatOfCamera*rotMat;
	Cc3dVector4 up=cc3dv4(newModelMatOfCamera.getAt(4),newModelMatOfCamera.getAt(5),newModelMatOfCamera.getAt(6),newModelMatOfCamera.getAt(7));
	Cc3dVector4 dir=cc3dv4(newModelMatOfCamera.getAt(8),newModelMatOfCamera.getAt(9),newModelMatOfCamera.getAt(10),newModelMatOfCamera.getAt(11))*(-1);
	Cc3dVector4 eyepos=cc3dv4(newModelMatOfCamera.getAt(12),newModelMatOfCamera.getAt(13),newModelMatOfCamera.getAt(14),newModelMatOfCamera.getAt(15));
	up.setw(0);
	dir.setw(0);
	eyepos.setw(1);
	//update camera
	m_eyePos=eyepos;
	m_center=m_eyePos+dir;
	m_up=up;

}