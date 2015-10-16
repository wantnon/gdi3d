#include "StdAfx.h"
#include "myFuncsForWin.h"


void TextOut(HDC hdc,int x,int y,const string&str){
	WCHAR wcstr[MAX_WCSTR_LEN];
	MultiByteToWideChar(0,0,str.c_str(),-1,wcstr,MAX_WCSTR_LEN);
	TextOut(hdc,x,y,wcstr,(int)str.size());
}