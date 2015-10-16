//
//  c3dLog.cpp
//  HelloCpp
//
//  Created by yang chao (wantnon) on 14-1-1.
//
//
#include "StdAfx.h"
#include "c3dLog.h"
void c3dLog(const char * pszFormat, ...)
{
    printf("c3d: ");
    char szBuf[maxC3dLogLen+1] = {0};
    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf(szBuf, maxC3dLogLen, pszFormat, ap);
    va_end(ap);
    printf("%s", szBuf);
    printf("\n");
}