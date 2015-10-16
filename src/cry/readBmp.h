
#include "Windows.h"
#include <iostream>
using namespace std;
class Crgb{
public:
	unsigned char m_r;
	unsigned char m_g;
	unsigned char m_b;
	void init(unsigned char r,unsigned char g,unsigned char b){
		m_r=r;
		m_g=g;
		m_b=b;
	}
};
class CmyBmp{
public:
	CmyBmp(){
		lineByte=0;
		pColorTable=NULL;
		pBmpBuf=NULL;

	}
	virtual~CmyBmp(){
		if(pBmpBuf){
			delete []pBmpBuf;
		}
		if(pColorTable){
			delete []pColorTable;
		}
	}
	bool readBmp(const char *bmpName) ;
	void drawBmp(HDC hdc);
	Crgb getPixelColor(int i,int j);
public:
	unsigned char *pBmpBuf;//读入图像数据的指针
	int bmpWidth;//图像的宽
	int bmpHeight;//图像的高
	RGBQUAD *pColorTable;//颜色表指针
	int biBitCount;//图像类型，每像素位数
public:
	int lineByte;//每行字节数
};
