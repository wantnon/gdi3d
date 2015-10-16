#include "StdAfx.h"
#include <stdio.h>
#include "readBmp.h"
bool CmyBmp::readBmp(const char *bmpName) //ref: http://wenku.baidu.com/view/901572d5360cba1aa811da61.html
{  
	//二进制读方式打开指定的图像文件
	FILE *fp=fopen(bmpName,"rb"); 
	if(fp==0) return 0; 
	//跳过位图文件头结构BITMAPFILEHEADER 
	fseek(fp, sizeof(BITMAPFILEHEADER),0); 
	//定义位图信息头结构变量，读取位图信息头进内存，存放在变量head中
	BITMAPINFOHEADER head;    
	fread(&head, sizeof(BITMAPINFOHEADER), 1,fp); 
	//获取图像宽、高、每像素所占位数等信息
	bmpWidth = head.biWidth; 
	bmpHeight = head.biHeight; 
	biBitCount = head.biBitCount;  
	//定义变量，计算图像每行像素所占的字节数（必须是4的倍数）
	lineByte=(bmpWidth * biBitCount/8+3)/4*4; 
	//灰度图像有颜色表，且颜色表表项为256 
	if(biBitCount==8){  
		//申请颜色表所需要的空间，读颜色表进内存
		pColorTable=new RGBQUAD[256];  
		fread(pColorTable,sizeof(RGBQUAD),256,fp); 
	}  
	//申请位图数据所需要的空间，读位图数据进内存
	pBmpBuf=new unsigned char[lineByte * bmpHeight]; 
	fread(pBmpBuf,1,lineByte * bmpHeight,fp); 
	//关闭文件
	fclose(fp); 

	return 1; 
}
void CmyBmp::drawBmp(HDC hdc){
	//ref: http://wenku.baidu.com/view/901572d5360cba1aa811da61.html
	for(int i=0;i<bmpHeight;i++){
		for(int j=0;j<bmpWidth;j++){
			unsigned char B=*(pBmpBuf+i*lineByte+j*3+0);
			unsigned char G=*(pBmpBuf+i*lineByte+j*3+1);
			unsigned char R=*(pBmpBuf+i*lineByte+j*3+2);
			SetPixel(hdc, j, i, RGB(R,G,B));
		}
	}

}
Crgb CmyBmp::getPixelColor(int i,int j){
	//ref: http://wenku.baidu.com/view/901572d5360cba1aa811da61.html
	Crgb rgb;
	if(i<0||i>=bmpHeight||j<0||j>=bmpWidth){
		rgb.init(255,255,255);
		return rgb;
	}
	unsigned char *base=pBmpBuf+i*lineByte+j*3;
	unsigned char B=*(base+0);
	unsigned char G=*(base+1);
	unsigned char R=*(base+2);
	rgb.init(R,G,B);
	return rgb;
}