#ifndef _texture_H_
#define _texture_H_

#include "readBmp.h"
#include "c3dVector.h"
class Ctexture{
public:
	Ctexture(){
		m_myBmp=NULL;
	}
	virtual~Ctexture(){
		if(m_myBmp){
			delete m_myBmp;
		}
	}
	
	bool initWithFile(const string&bmpFileName){//if succ return true
		m_myBmp=new CmyBmp();
		bool succ=m_myBmp->readBmp(bmpFileName.c_str());
		return succ;
	}
	Cc3dVector4 getColor(const Cc3dVector2&texCoord)const {
		float s=texCoord.x();
		float t=1-texCoord.y();
		s=s-floor(s);
		t=t-floor(t);
		float j=s*m_myBmp->bmpWidth;
		float i=t*m_myBmp->bmpHeight;
		//bilinear interpolation
		Cc3dVector4 color;
		{
			float jL,jR,iU,iD;
			jL=floor(j-0.5)+0.5;
			jR=jL+1;
			iU=floor(i-0.5)+0.5;
			iD=iU+1;
			float djL=j-jL;
			float djR=jR-j;
			float diU=i-iU;
			float diD=iD-i;
			int jL_int=floor(jL);
			int jR_int=jL_int+1;
			int iU_int=floor(iU);
			int iD_int=iU_int+1;
			Crgb LUcolor=m_myBmp->getPixelColor(max(iU_int,0),max(jL_int,0));
			Crgb LDcolor=m_myBmp->getPixelColor(min(iD_int,m_myBmp->bmpHeight-1),max(jL_int,0));
			Crgb RUcolor=m_myBmp->getPixelColor(max(iU_int,0),min(jR_int,m_myBmp->bmpWidth-1));
			Crgb RDcolor=m_myBmp->getPixelColor(min(iD_int,m_myBmp->bmpHeight-1),min(jR_int,m_myBmp->bmpWidth-1));
			Cc3dVector4 LUcolorf((float)LUcolor.m_r/255,(float)LUcolor.m_g/255,(float)LUcolor.m_b/255,1);
			Cc3dVector4 LDcolorf((float)LDcolor.m_r/255,(float)LDcolor.m_g/255,(float)LDcolor.m_b/255,1);
			Cc3dVector4 RUcolorf((float)RUcolor.m_r/255,(float)RUcolor.m_g/255,(float)RUcolor.m_b/255,1);
			Cc3dVector4 RDcolorf((float)RDcolor.m_r/255,(float)RDcolor.m_g/255,(float)RDcolor.m_b/255,1);
			Cc3dVector4 Ucolorf,Dcolorf,CenterColor;
			Ucolorf=LUcolorf*djR+RUcolorf*djL;
			Dcolorf=LDcolorf*djR+RDcolorf*djL;
			CenterColor=Ucolorf*diD+Dcolorf*diU;
			color=CenterColor;
		}


		return color;
	
	}
protected:
	CmyBmp* m_myBmp;

};

#endif