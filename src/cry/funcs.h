#ifndef _funcs_H_
#define _funcs_H_
#include <string>
#include <iostream>
using namespace std;
#include "mesh.h"
#include "c3dMath.h"
string numberToStr(int n);
int strToNumber(const string&numStr);
int charToNumber(char c);
vector<string> divideStr(const string&str,const string&dividerChars);
Cc3dVector4 fscanVector4(FILE * fp);
Cc3dVector4 fscanVector3(FILE * fp);
Cc3dVector2 fscanVector2(FILE * fp);
float fscanFloat(FILE * fp);
int fscanInt(FILE * fp);
string fscanStr(FILE*fp,const string&expectedStr="");
string fscanOneQuotation(FILE * fp);
void makeCube(Cmesh*mesh);
void makeSphere(Cmesh*mesh,int nSlice,int nStack);
vector<int> getThreeValuesDownOrderIndexList(float a,float b,float c);
Cvert linePHighPLowIntersectWithHorizontalLineAcrossPMid(const Cvert&vHigh,const Cvert&vLow,const Cvert&vMid);
double lineIntersectWithHorizontalLine(const Cc3dVector4&p1OnLine,const Cc3dVector4&p2OnLine,double yHorizontalLine);
void moveBoolListToTrueTrueTrueOrFalseFalseFalseOrTrueFalseFalseOrFalseTrueTrueMeanwhileMoveTheVertListAtTheSameTime(vector<bool>&bList,vector<Cvert>&_vList);






#endif