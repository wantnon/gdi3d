#include "StdAfx.h"
#include "funcs.h"

static char buffer[1024]={0};//used for fscan string
string numberToStr(int n){//int to string
    char numbuf[128];
    sprintf(numbuf, "%i", n);//there is no itoa in xcode
    string numStr=numbuf;
    return numStr;
}
int charToNumber(char c){
	assert('0'<'9');
	assert(c>='0');
	assert(c<='9');
	return (c-'0');
}
int strToNumber(const string&numStr){
	int number=0;
	int len=(int)numStr.size();
	for(int i=0;i<len;i++){
		char c=numStr[i];
		int num=charToNumber(c);
		number*=10;
		number+=num;
	}//got number
	return number;
}
vector<string> divideStr(const string&str,const string&dividerChars){
	const int dividerCharCount=(int)dividerChars.size();
	vector<string> strList;
	string t_str;
	int len=(int)str.size();
	for(int i=0;i<len;i++){
		char c=str[i];
		bool cIsDividerCh=false;
		for(int j=0;j<dividerCharCount;j++){
			char divierCh=dividerChars[j];
			if(c==divierCh){
				cIsDividerCh=true;
				break;
			}
		}//got cIsDividerCh
		if(cIsDividerCh==false){
			t_str+=c;
		}else{
			strList.push_back(t_str);
			t_str.clear();
		}
	}
	if(t_str.empty()==false){
		strList.push_back(t_str);
	}//got strList;
	return strList;

}
void moveBoolListToTrueTrueTrueOrFalseFalseFalseOrTrueFalseFalseOrFalseTrueTrueMeanwhileMoveTheVertListAtTheSameTime(vector<bool>&bList,vector<Cvert>&vList){
	if(bList[0]==bList[1]&&bList[0]==bList[2]){//quick return
		return;
	}
	int sumb=0;
	for(int i=0;i<3;i++){
		bool b=bList[i];
		sumb+=(int)b;
	}//got bList and sumb
	if(sumb==3||sumb==0){//all true or all false
		//because quick return, so this branch can not be reach
		assert(false);
	}else if(sumb==2){//two true and one false
		if(bList[1]==false){// ( true , false , true )
			//make it become ( false, true, true )
			vector<Cvert> t_vList;
			t_vList.push_back(vList[1]);
			t_vList.push_back(vList[2]);
			t_vList.push_back(vList[0]);
			vList=t_vList;
			bList[0]=false;
			bList[1]=true;
			bList[2]=true;
		}else if(bList[2]==false){// ( true , true , false )
			//make it become ( false, true, true )
			vector<Cvert> t_vList;
			t_vList.push_back(vList[2]);
			t_vList.push_back(vList[0]);
			t_vList.push_back(vList[1]);
			vList=t_vList;
			bList[0]=false;
			bList[1]=true;
			bList[2]=true;
		}
	}else if(sumb==1){//one true and two false
		if(bList[1]){// ( false , true , false )
			//make it become ( true , false , false )
			vector<Cvert> t_vList;
			t_vList.push_back(vList[1]);
			t_vList.push_back(vList[2]);
			t_vList.push_back(vList[0]);
			vList=t_vList;
			bList[0]=true;
			bList[1]=false;
			bList[2]=false;
		}else if(bList[2]){//( false , false , true )
			//make it become ( true , false , false )
			vector<Cvert> t_vList;
			t_vList.push_back(vList[2]);
			t_vList.push_back(vList[0]);
			t_vList.push_back(vList[1]);
			vList=t_vList;
			bList[0]=true;
			bList[1]=false;
			bList[2]=false;
		}
	}else{//it is not possible to enter this branch
		assert(false);
	}
}
vector<int> getThreeValuesDownOrderIndexList(float a,float b,float c){
	vector<int> rs;
	int indexOfA=0;
	int indexOfB=1;
	int indexOfC=2;
	bool ABigThanB=(a>b);
	bool ABigThanC=(a>c);
	bool BBigThanC=(b>c);
	if(ABigThanB!=ABigThanC){//b and c is on the different side of a
		if(BBigThanC){//b>c
			rs.push_back(indexOfB);
			rs.push_back(indexOfA);
			rs.push_back(indexOfC);
		}else{
			rs.push_back(indexOfC);
			rs.push_back(indexOfA);
			rs.push_back(indexOfB);
		}
	}else{//b and c is on the same side of a, namely a is min or max
		if(ABigThanB){//a is max
			if(BBigThanC){//a>b>c
				rs.push_back(indexOfA);
				rs.push_back(indexOfB);
				rs.push_back(indexOfC);
			}else{//a>c>b
				rs.push_back(indexOfA);
				rs.push_back(indexOfC);
				rs.push_back(indexOfB);
			}
		}else{//a is min
			if(BBigThanC){//b>c>a
				rs.push_back(indexOfB);
				rs.push_back(indexOfC);
				rs.push_back(indexOfA);
			}else{//c>b>a
				rs.push_back(indexOfC);
				rs.push_back(indexOfB);
				rs.push_back(indexOfA);
			}

		}
	}
	return rs;
}
Cvert linePHighPLowIntersectWithHorizontalLineAcrossPMid(const Cvert&vHigh,const Cvert&vLow,const Cvert&vMid)
	//return intersect point
{
	//----calculate x,y
	const double x1=vHigh.m_pos.x();
	const double y1=vHigh.m_pos.y();
	const double x2=vLow.m_pos.x();
	const double y2=vLow.m_pos.y();
	const double x3=vMid.m_pos.x();
	const double y3=vMid.m_pos.y();
	double x,y;
	if(y1==y2){
		x=x3;
		y=y3;
	}else{
		y=y3;
		x=(y3-y1)*(x2-x1)/(y2-y1)+x1;
	}

	
	//----make v
	Cvert v;
	v.m_pos=Cc3dVector4(x,y,0,1);

	return v;

}

double lineIntersectWithHorizontalLine(const Cc3dVector4&p1OnLine,const Cc3dVector4&p2OnLine,double yHorizontalLine){//return intersect x
	//----calculate x,y
	const double x1=p1OnLine.x();
	const double y1=p1OnLine.y();
	const double x2=p2OnLine.x();
	const double y2=p2OnLine.y();
	const double y=yHorizontalLine;
	double x;
	if(y1==y2){//line is horizontal line too, can not calculate x
		return 0;//no matter return what
	}else{
		x=(y-y1)*(x2-x1)/(y2-y1)+x1;
	}

	return x;
	
}



void makeCube(Cmesh*mesh){
	assert(mesh);
	assert(mesh->m_positionList.empty());
	const float L=1;
	const float d=L/2;
	vector<Cc3dVector4> posList;
	vector<Cc3dIDTriangle> IDtriList;
	vector<Cc3dVector4> normList;
	vector<Cc3dVector4> colorList;
	vector<Cc3dVector4> colorAddList;
	vector<Cc3dVector2> texCoordList;
	vector<int> textureIDList;
	vector<int> materialIDList;
	vector<bool> isCullBackList;
	//front face
	{
		posList.push_back(cc3dv4(-d,-d,+d,1));int vid0=posList.size()-1;
		posList.push_back(cc3dv4(+d,-d,+d,1));int vid1=posList.size()-1;
		posList.push_back(cc3dv4(+d,+d,+d,1));int vid2=posList.size()-1;
		posList.push_back(cc3dv4(-d,+d,+d,1));int vid3=posList.size()-1;
		textureIDList.push_back(0);
		textureIDList.push_back(0);
		textureIDList.push_back(0);
		textureIDList.push_back(0);
		materialIDList.push_back(0);
		materialIDList.push_back(0);
		materialIDList.push_back(0);
		materialIDList.push_back(0);
		isCullBackList.push_back(true);
		isCullBackList.push_back(true);
		isCullBackList.push_back(true);
		isCullBackList.push_back(true);
		normList.push_back(cc3dv4(0,0,1,0));
		normList.push_back(cc3dv4(0,0,1,0));
		normList.push_back(cc3dv4(0,0,1,0));
		normList.push_back(cc3dv4(0,0,1,0));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		texCoordList.push_back(Cc3dVector2(0,0));
		texCoordList.push_back(Cc3dVector2(1,0));
		texCoordList.push_back(Cc3dVector2(1,1));
		texCoordList.push_back(Cc3dVector2(0,1));
		IDtriList.push_back(Cc3dIDTriangle(vid0,vid1,vid2));
		IDtriList.push_back(Cc3dIDTriangle(vid0,vid2,vid3));
	}
	//back face
	{
		posList.push_back(cc3dv4(-d,-d,-d,1));int vid0=posList.size()-1;
		posList.push_back(cc3dv4(-d,+d,-d,1));int vid1=posList.size()-1;
		posList.push_back(cc3dv4(+d,+d,-d,1));int vid2=posList.size()-1;
		posList.push_back(cc3dv4(+d,-d,-d,1));int vid3=posList.size()-1;
		textureIDList.push_back(0);
		textureIDList.push_back(0);
		textureIDList.push_back(0);
		textureIDList.push_back(0);
		materialIDList.push_back(0);
		materialIDList.push_back(0);
		materialIDList.push_back(0);
		materialIDList.push_back(0);
		isCullBackList.push_back(true);
		isCullBackList.push_back(true);
		isCullBackList.push_back(true);
		isCullBackList.push_back(true);
		normList.push_back(cc3dv4(0,0,-1,0));
		normList.push_back(cc3dv4(0,0,-1,0));
		normList.push_back(cc3dv4(0,0,-1,0));
		normList.push_back(cc3dv4(0,0,-1,0));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		texCoordList.push_back(Cc3dVector2(0,0));
		texCoordList.push_back(Cc3dVector2(1,0));
		texCoordList.push_back(Cc3dVector2(1,1));
		texCoordList.push_back(Cc3dVector2(0,1));
		IDtriList.push_back(Cc3dIDTriangle(vid0,vid1,vid2));
		IDtriList.push_back(Cc3dIDTriangle(vid0,vid2,vid3));
	}
	//left face
	{
		posList.push_back(cc3dv4(-d,-d,+d,1));int vid0=posList.size()-1;
		posList.push_back(cc3dv4(-d,+d,+d,1));int vid1=posList.size()-1;
		posList.push_back(cc3dv4(-d,+d,-d,1));int vid2=posList.size()-1;
		posList.push_back(cc3dv4(-d,-d,-d,1));int vid3=posList.size()-1;
		textureIDList.push_back(0);
		textureIDList.push_back(0);
		textureIDList.push_back(0);
		textureIDList.push_back(0);
		materialIDList.push_back(0);
		materialIDList.push_back(0);
		materialIDList.push_back(0);
		materialIDList.push_back(0);
		isCullBackList.push_back(true);
		isCullBackList.push_back(true);
		isCullBackList.push_back(true);
		isCullBackList.push_back(true);
		normList.push_back(cc3dv4(-1,0,0,0));
		normList.push_back(cc3dv4(-1,0,0,0));
		normList.push_back(cc3dv4(-1,0,0,0));
		normList.push_back(cc3dv4(-1,0,0,0));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		texCoordList.push_back(Cc3dVector2(0,0));
		texCoordList.push_back(Cc3dVector2(1,0));
		texCoordList.push_back(Cc3dVector2(1,1));
		texCoordList.push_back(Cc3dVector2(0,1));
		IDtriList.push_back(Cc3dIDTriangle(vid0,vid1,vid2));
		IDtriList.push_back(Cc3dIDTriangle(vid0,vid2,vid3));

	}
	//right face
	{
		posList.push_back(cc3dv4(+d,-d,+d,1));int vid0=posList.size()-1;
		posList.push_back(cc3dv4(+d,-d,-d,1));int vid1=posList.size()-1;
		posList.push_back(cc3dv4(+d,+d,-d,1));int vid2=posList.size()-1;
		posList.push_back(cc3dv4(+d,+d,+d,1));int vid3=posList.size()-1;
		textureIDList.push_back(0);
		textureIDList.push_back(0);
		textureIDList.push_back(0);
		textureIDList.push_back(0);
		materialIDList.push_back(0);
		materialIDList.push_back(0);
		materialIDList.push_back(0);
		materialIDList.push_back(0);
		isCullBackList.push_back(true);
		isCullBackList.push_back(true);
		isCullBackList.push_back(true);
		isCullBackList.push_back(true);
		normList.push_back(cc3dv4(1,0,0,0));
		normList.push_back(cc3dv4(1,0,0,0));
		normList.push_back(cc3dv4(1,0,0,0));
		normList.push_back(cc3dv4(1,0,0,0));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		texCoordList.push_back(Cc3dVector2(0,0));
		texCoordList.push_back(Cc3dVector2(1,0));
		texCoordList.push_back(Cc3dVector2(1,1));
		texCoordList.push_back(Cc3dVector2(0,1));
		IDtriList.push_back(Cc3dIDTriangle(vid0,vid1,vid2));
		IDtriList.push_back(Cc3dIDTriangle(vid0,vid2,vid3));

	}
	//up face
	{
		posList.push_back(cc3dv4(-d,+d,+d,1));int vid0=posList.size()-1;
		posList.push_back(cc3dv4(+d,+d,+d,1));int vid1=posList.size()-1;
		posList.push_back(cc3dv4(+d,+d,-d,1));int vid2=posList.size()-1;
		posList.push_back(cc3dv4(-d,+d,-d,1));int vid3=posList.size()-1;
		textureIDList.push_back(0);
		textureIDList.push_back(0);
		textureIDList.push_back(0);
		textureIDList.push_back(0);
		materialIDList.push_back(0);
		materialIDList.push_back(0);
		materialIDList.push_back(0);
		materialIDList.push_back(0);
		isCullBackList.push_back(true);
		isCullBackList.push_back(true);
		isCullBackList.push_back(true);
		isCullBackList.push_back(true);
		normList.push_back(cc3dv4(0,1,0,0));
		normList.push_back(cc3dv4(0,1,0,0));
		normList.push_back(cc3dv4(0,1,0,0));
		normList.push_back(cc3dv4(0,1,0,0));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		texCoordList.push_back(Cc3dVector2(0,0));
		texCoordList.push_back(Cc3dVector2(1,0));
		texCoordList.push_back(Cc3dVector2(1,1));
		texCoordList.push_back(Cc3dVector2(0,1));
		IDtriList.push_back(Cc3dIDTriangle(vid0,vid1,vid2));
		IDtriList.push_back(Cc3dIDTriangle(vid0,vid2,vid3));

	}
	//dn face
	{
		posList.push_back(cc3dv4(-d,-d,+d,1));int vid0=posList.size()-1;
		posList.push_back(cc3dv4(-d,-d,-d,1));int vid1=posList.size()-1;
		posList.push_back(cc3dv4(+d,-d,-d,1));int vid2=posList.size()-1;
		posList.push_back(cc3dv4(+d,-d,+d,1));int vid3=posList.size()-1;
		textureIDList.push_back(0);
		textureIDList.push_back(0);
		textureIDList.push_back(0);
		textureIDList.push_back(0);
		materialIDList.push_back(0);
		materialIDList.push_back(0);
		materialIDList.push_back(0);
		materialIDList.push_back(0);
		isCullBackList.push_back(true);
		isCullBackList.push_back(true);
		isCullBackList.push_back(true);
		isCullBackList.push_back(true);
		normList.push_back(cc3dv4(0,-1,0,0));
		normList.push_back(cc3dv4(0,-1,0,0));
		normList.push_back(cc3dv4(0,-1,0,0));
		normList.push_back(cc3dv4(0,-1,0,0));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		colorAddList.push_back(cc3dv4(1,1,1,1));
		texCoordList.push_back(Cc3dVector2(0,0));
		texCoordList.push_back(Cc3dVector2(1,0));
		texCoordList.push_back(Cc3dVector2(1,1));
		texCoordList.push_back(Cc3dVector2(0,1));
		IDtriList.push_back(Cc3dIDTriangle(vid0,vid1,vid2));
		IDtriList.push_back(Cc3dIDTriangle(vid0,vid2,vid3));

	}//*/


	//----assign to subMeshData
	mesh->m_positionList=posList;
	mesh->m_IDtriList=IDtriList;
	mesh->m_normalList=normList;
	mesh->m_colorList=colorList;
	mesh->m_textureIDList=textureIDList;
	mesh->m_materialIDList=materialIDList;
	mesh->m_texCoordList=texCoordList;
	mesh->m_isCullBackList=isCullBackList;
	mesh->m_colorAddList=colorAddList;

}

void makeSphere(Cmesh*mesh,int nSlice,int nStack){
	assert(mesh);
	assert(mesh->m_positionList.empty());

	const float r=0.5;



    float dA=360.0/nSlice;

    float dB=180.0/nStack;

    const float PIDIV180=M_PI/180;

    vector<Cc3dVector4> positionList;

    vector<Cc3dVector2> texCoordList;

    vector<Cc3dVector4> normalList;

    vector<Cc3dVector4> colorList;

	vector<Cc3dVector4> colorAddList;

    vector<Cc3dIDTriangle> IDtriList;

	vector<int> textureIDList;

	vector<int> materialIDList;

	vector<bool> isCullBackList;
    //generate positionList, texCoordList, normalList, colorList

    for(int i=0;i<=nStack;i++){

        float B=-90+i*dB;

        float y=r*sinf(B*PIDIV180);

        float cosB=cosf(B*PIDIV180);

        for (int j=0; j<=nSlice; j++) {

            float A=j*dA;

            float R=r*cosB;

            float x=R*cosf(A*PIDIV180);

            float z=R*sinf(A*PIDIV180);

            Cc3dVector4 position(x,y,z,1);

            float s=(float)j/nSlice+0.25;

            float t=1-(float)i/nStack;

            Cc3dVector2 texCoord(s,t);

            Cc3dVector4 normal=normalize(cc3dv4(position.x(), position.y(), position.z(), 0));

            Cc3dVector4 color=cc3dv4(1,1,1,1);

			Cc3dVector4 colorAdd=cc3dv4(1,1,1,1);

            positionList.push_back(position);

            texCoordList.push_back(texCoord);

            normalList.push_back(normal);

            colorList.push_back(color);

			textureIDList.push_back(0);

			materialIDList.push_back(0);

			isCullBackList.push_back(true);
			colorAddList.push_back(colorAdd);



        }

    }

    //generate IDtriList

    for(int i=0;i<nStack;i++){

        for(int j=0;j<nSlice;j++){

            int vID_rd=(nSlice+1)*i+j;

            int vID_ld=vID_rd+1;

            int vID_lu=vID_ld+(nSlice+1);

            int vID_ru=vID_lu-1;

            Cc3dIDTriangle IDtri0(vID_ld,vID_rd,vID_ru);

            Cc3dIDTriangle IDtri1(vID_ld,vID_ru,vID_lu);

            IDtriList.push_back(IDtri0);

            IDtriList.push_back(IDtri1);

        }

    }

	//----assign to subMeshData
	mesh->m_positionList=positionList;
	mesh->m_IDtriList=IDtriList;
	mesh->m_normalList=normalList;
	mesh->m_colorList=colorList;
	mesh->m_texCoordList=texCoordList;
	mesh->m_textureIDList=textureIDList;
	mesh->m_materialIDList=materialIDList;
	mesh->m_isCullBackList=isCullBackList;
	mesh->m_colorAddList=colorAddList;


}

Cc3dVector4 fscanVector4(FILE * fp){
	assert(fp);
	float arr[4];
	for(int i=0;i<4;i++){
		fscanf(fp,"%f",&arr[i]);
	}
	return Cc3dVector4(arr);
}
Cc3dVector4 fscanVector3(FILE * fp){
	assert(fp);
	float arr[3];
	for(int i=0;i<3;i++){
		fscanf(fp,"%f",&arr[i]);
	}
	return Cc3dVector4(arr);
}
Cc3dVector2 fscanVector2(FILE * fp){
	assert(fp);
	float arr[2];
	for(int i=0;i<2;i++){
		fscanf(fp,"%f",&arr[i]);
	}
	return Cc3dVector2(arr);
}
float fscanFloat(FILE * fp){
	assert(fp);
	float temp=0;
	fscanf(fp,"%f",&temp);
	return temp;

}
int fscanInt(FILE * fp){
	assert(fp);
	int temp=0;
	fscanf(fp,"%i",&temp);
	return temp;
}
string fscanStr(FILE*fp,const string&expectedStr){
	assert(fp);
	fscanf(fp,"%s",buffer);
	string str=buffer;
	if(expectedStr.empty()==false){
		assert(str==expectedStr);
	}
	return str;
}
string fscanOneQuotation(FILE * fp){
	assert(fp);
	string quoatation="";
	fscanf(fp,"%s",buffer);
	string beginStr=buffer;
	assert(beginStr[0]=='\"');
	quoatation+=beginStr;
	while(1){
		char endCh=quoatation[(int)quoatation.size()-1];
		if(endCh=='\"'){
			break;
		}
		fscanf(fp,"%s",buffer);
		string str=buffer;
		quoatation+=str;
		
	}//got quoatation
	return quoatation;

}