#include "StdAfx.h"
#include "gdi3d.h"
Cgdi3dScene* gdi3dScene=NULL;

Cvert vertexShaderProgram(const Cc3dMatrix4&modelMat,const Cc3dCamera*camera,const Cc3dLightSource*lightSource,const Cc3dMaterial*material,const Cvert&v)
{
	const Cc3dMatrix4&viewMat=camera->calculateViewMat();
	const Cc3dMatrix4&projMat=camera->calculateProjectionMat();
	const Cc3dMatrix4&normMat=transpose(inverse(modelMat));
	const Cc3dVector4&ambientML=perComponentProduct(lightSource->m_ambient,material->m_ambient);
	const Cc3dVector4&diffuseML=perComponentProduct(lightSource->m_diffuse,material->m_diffuse);
	const Cc3dVector4&specularML=perComponentProduct(lightSource->m_specular,material->m_specular);
	const float shininess=material->m_shininess;
	//----transform pos
	Cc3dVector4 pos_world=modelMat*v.m_pos;
	//----transform norm
	Cc3dVector4 norm_world=normalize(normMat*v.m_norm);
	//----calculate lighting
	Cc3dVector4 posToLight=normalize(lightSource->m_pos-pos_world);
	float normDotPosToLight = max(0.0, dot(norm_world, posToLight));
	Cc3dVector4 diffuseColor= diffuseML*normDotPosToLight;
	diffuseColor.seta(diffuseML.a());
	//----calculate ambient
	Cc3dVector4 ambientColor=ambientML;
	ambientColor.seta(1);
	//----calculate specular

	Cc3dVector4 eyePos_world=camera->getEyePos();
	Cc3dVector4 posToEye=normalize(eyePos_world-pos_world);
	Cc3dVector4 halfVector=(posToLight+posToEye)*0.5;
	float normDotHalfVector=dot(norm_world,halfVector);
	float pf=normDotHalfVector==0?0:pow(normDotHalfVector,shininess);

	Cc3dVector4 specularColor=specularML*pf;
	specularColor.setw(1);

	//----calculate mainColor
	Cc3dVector4 mainColor=diffuseColor+ambientColor;
	Cc3dVector4 colorAdd=specularColor;

	//----gl_Position
	Cc3dVector4 gl_Position=projMat*viewMat*pos_world;
	//----make vsOutput
	Cvert transformedV=v;//copy v
	transformedV.m_pos=gl_Position;
	transformedV.m_color=mainColor;
	transformedV.m_colorAdd=colorAdd;
	return transformedV;
}
Cfrag fragShaderProgram(const Cvert&interpolatedV,const Ctexture*texture){
	Cfrag frag;
	frag.m_pos=interpolatedV.m_pos;
	frag.m_color=perComponentProduct(interpolatedV.m_color,texture->getColor(interpolatedV.m_texCoord));
	frag.m_color=frag.m_color+interpolatedV.m_colorAdd;//must add after mul textureColor
	return frag;
}

void Cgdi3dScene::makeSampleScene_girl(){
	resetScene();
	loadModelMyFormat("res/MyFormatModels/girl","girl.txt",m_mesh);
	m_modelMat=calculateScaleMatrix(0.045,0.045,0.045)*calculateTranslateMatrix(cc3dv4(0,23,0,0))*calculateRotationMatrix(cc3dv4(1,0,0,0),cos(-90*M_PI/180),sin(-90*M_PI/180));
	m_camera->setEyePos(cc3dv4(0,0,3,1));
	m_camera->setCenter(cc3dv4(0,0,0,1));
	m_camera->setUp(cc3dv4(0,1,0,0));

	storeInitialCameraSetting();
	storeInitialModelMat();
}
void Cgdi3dScene::makeSampleScene_teaport(){
	resetScene();
	loadModelASE("res/ASEModels","teaport.ASE",m_mesh);
	m_modelMat=calculateScaleMatrix(0.02,0.02,0.02)*calculateTranslateMatrix(cc3dv4(0,-20,0,0))*calculateRotationMatrix(cc3dv4(1,0,0,0),cos(-90*M_PI/180),sin(-90*M_PI/180));
	m_camera->setEyePos(cc3dv4(0,0,3,1));
	m_camera->setCenter(cc3dv4(0,0,0,1));
	m_camera->setUp(cc3dv4(0,1,0,0));

	storeInitialCameraSetting();
	storeInitialModelMat();
}
void Cgdi3dScene::makeSampleScene_cube(){
	resetScene();
	//make mesh
	makeCube(m_mesh);
	//create material
	{
		Cc3dMaterial*material=new Cc3dMaterial();
		m_materialList.push_back(material);
	}
	//create texture
	{
		Ctexture*texture=new Ctexture();
		string texFileName="res/tex/tex.bmp";
		bool succ=texture->initWithFile(texFileName.c_str());
		assert(succ);
		m_textureList.push_back(texture);
	}
	m_modelMat=unitMat();

	m_camera->setEyePos(cc3dv4(0,0,3,1));
	m_camera->setCenter(cc3dv4(0,0,0,1));
	m_camera->setUp(cc3dv4(0,1,0,0));

	storeInitialCameraSetting();
	storeInitialModelMat();

}
void Cgdi3dScene::makeSampleScene_ball(){
	resetScene();
	//make mesh
	makeSphere(m_mesh,30,30);
	//create material
	{
		Cc3dMaterial*material=new Cc3dMaterial();
		material->m_specular=cc3dv4(0.3,0.3,0.3,1);
		m_materialList.push_back(material);
	}
	//create texture
	{
		Ctexture*texture=new Ctexture();
		string texFileName="res/tex/basketballTex.bmp";
		bool succ=texture->initWithFile(texFileName.c_str());
		assert(succ);
		m_textureList.push_back(texture);
	}

	m_modelMat=unitMat();

	m_camera->setEyePos(cc3dv4(0,0,2.5,1));
	m_camera->setCenter(cc3dv4(0,0,0,1));
	m_camera->setUp(cc3dv4(0,1,0,0));

	storeInitialCameraSetting();
	storeInitialModelMat();
}
void Cgdi3dScene::resetScene(){
	m_mesh->clearAll();
	for(int i=0;i<(int)m_materialList.size();i++){
		delete m_materialList[i];
	}m_materialList.clear();
	for(int i=0;i<(int)m_textureList.size();i++){
		delete m_textureList[i];
	}m_textureList.clear();

}
void Cgdi3dScene::storeInitialModelMat(){
	m_initialModelMat=m_modelMat;
}
void Cgdi3dScene::restoreToInitialModelMat(){
	m_modelMat=m_initialModelMat;
}
void Cgdi3dScene::storeInitialCameraSetting(){
	m_initialCameraEyePos=m_camera->getEyePos();
	m_initialCameraUp=m_camera->getUp();
	m_initialCameraCenter=m_camera->getCenter();
}
void Cgdi3dScene::restoreToInitialCameraSettting(){
	m_camera->setEyePos(m_initialCameraEyePos);
	m_camera->setUp(m_initialCameraUp);
	m_camera->setCenter(m_initialCameraCenter);
}
void Cgdi3dScene::loadModelASE(const string&folderPath,const string&fileName,Cmesh*mesh){
	//ref: http://wenku.baidu.com/link?url=CiPPM61Smtd4GD835IsuSp_PGewAHZxFkxMX2f2vql2ovgU1YeX1iKG0HPyohZYPgwibqTbk2_48WAMbpOO9vf94fc8PG7YLb7y7IIpKbSW
	assert(mesh);
	assert(mesh->m_positionList.empty());

	vector<Ctexture*>&textureList=m_textureList;
	vector<Cc3dMaterial*>&materialList=m_materialList;

	//------------------full path of file
	string fileFullPath=folderPath+"/"+fileName;
	//open file
	FILE  *fp=NULL;
	fp=fopen(fileFullPath.c_str(),"r");// write
	if(fp==NULL){
		cout<<"Open "<<fileFullPath<<" failed!"<<endl;
		assert(false);
	}
	//------------------read file
	vector<Cc3dVector4> posList;
	vector<Cc3dIDTriangle> faceList;
	int materialRef=0;
	vector<Cc3dVector2> tvertexList;
	vector<Cc3dIDTriangle> tfaceList;
	vector<Cc3dVector4> faceNormList;
	vector<Cc3dVector4> faceV0NormList;
	vector<Cc3dVector4> faceV1NormList;
	vector<Cc3dVector4> faceV2NormList;
	vector<Cc3dVector4> ambientList;
	vector<Cc3dVector4> diffuseList;
	vector<float> shineStrengthList;
	vector<float> shineList;
	vector<Cc3dVector4> specularList;
	vector<string> texFileNameList;
	{
		fscanStr(fp,"*3DSMAX_ASCIIEXPORT");
		fscanInt(fp);
		fscanStr(fp,"*COMMENT");
		fscanOneQuotation(fp);
		fscanStr(fp,"*SCENE");
		fscanStr(fp,"{");
		fscanStr(fp,"*SCENE_FILENAME");
		fscanOneQuotation(fp);
		fscanStr(fp,"*SCENE_FIRSTFRAME");
		fscanInt(fp);
		fscanStr(fp,"*SCENE_LASTFRAME");
		fscanInt(fp);
		fscanStr(fp,"*SCENE_FRAMESPEED");
		fscanInt(fp);
		fscanStr(fp,"*SCENE_TICKSPERFRAME");
		fscanInt(fp);
		fscanStr(fp,"*SCENE_BACKGROUND_STATIC");
		fscanVector3(fp);
		fscanStr(fp,"*SCENE_AMBIENT_STATIC");
		fscanVector3(fp);
		fscanStr(fp,"}");
		fscanStr(fp,"*MATERIAL_LIST");
		fscanStr(fp,"{");
		fscanStr(fp,"*MATERIAL_COUNT");
		int materialCount=fscanInt(fp);
		for(int materialIndex=0;materialIndex<materialCount;materialIndex++){
			fscanStr(fp,"*MATERIAL");
			int materialID=fscanInt(fp);
			assert(materialID==materialIndex);
			fscanStr(fp,"{");
			fscanStr(fp,"*MATERIAL_NAME"); fscanOneQuotation(fp);
			fscanStr(fp,"*MATERIAL_CLASS"); fscanOneQuotation(fp);
			fscanStr(fp,"*MATERIAL_AMBIENT"); Cc3dVector4 ambient=fscanVector3(fp); ambient.setw(1);
			fscanStr(fp,"*MATERIAL_DIFFUSE"); Cc3dVector4 diffuse=fscanVector3(fp); diffuse.setw(1);
			fscanStr(fp,"*MATERIAL_SPECULAR"); Cc3dVector4 specular=fscanVector3(fp); specular.setw(1);
			fscanStr(fp,"*MATERIAL_SHINE"); float shine=fscanFloat(fp);
			fscanStr(fp,"*MATERIAL_SHINESTRENGTH"); float shineStrength=fscanFloat(fp);
			fscanStr(fp,"*MATERIAL_TRANSPARENCY"); fscanFloat(fp);
			fscanStr(fp,"*MATERIAL_WIRESIZE"); fscanFloat(fp);
			fscanStr(fp,"*MATERIAL_SHADING"); fscanStr(fp);
			fscanStr(fp,"*MATERIAL_XP_FALLOFF"); fscanFloat(fp);
			fscanStr(fp,"*MATERIAL_SELFILLUM"); fscanFloat(fp);
			fscanStr(fp,"*MATERIAL_FALLOFF"); fscanStr(fp);
			fscanStr(fp,"*MATERIAL_XP_TYPE"); fscanStr(fp);
			fscanStr(fp,"*MAP_DIFFUSE"); fscanStr(fp,"{");
			fscanStr(fp,"*MAP_NAME"); fscanOneQuotation(fp);
			fscanStr(fp,"*MAP_CLASS"); fscanOneQuotation(fp);
			fscanStr(fp,"*MAP_SUBNO"); fscanInt(fp);
			fscanStr(fp,"*MAP_AMOUNT"); fscanFloat(fp);
			fscanStr(fp,"*BITMAP"); string texFilePathWithQuotation=fscanOneQuotation(fp);
			string texFilePath=texFilePathWithQuotation.substr(1,(int)texFilePathWithQuotation.size()-2);
			string dividerChars;
			dividerChars.push_back('/');
			dividerChars.push_back('\\');
			assert((int)dividerChars.size()==2);
			string texFileName=divideStr(texFilePath,dividerChars).back();
			fscanStr(fp,"*MAP_TYPE"); fscanStr(fp);
			fscanStr(fp,"*UVW_U_OFFSET"); fscanFloat(fp);
			fscanStr(fp,"*UVW_V_OFFSET"); fscanFloat(fp);
			fscanStr(fp,"*UVW_U_TILING"); fscanFloat(fp);
			fscanStr(fp,"*UVW_V_TILING"); fscanFloat(fp);
			fscanStr(fp,"*UVW_ANGLE"); fscanFloat(fp);
			fscanStr(fp,"*UVW_BLUR"); fscanFloat(fp);
			fscanStr(fp,"*UVW_BLUR_OFFSET"); fscanFloat(fp);
			fscanStr(fp,"*UVW_NOUSE_AMT"); fscanFloat(fp);
			fscanStr(fp,"*UVW_NOISE_SIZE"); fscanFloat(fp);
			fscanStr(fp,"*UVW_NOISE_LEVEL"); fscanInt(fp);
			fscanStr(fp,"*UVW_NOISE_PHASE"); fscanFloat(fp);
			fscanStr(fp,"*BITMAP_FILTER"); fscanStr(fp);
			fscanStr(fp,"}");
			fscanStr(fp,"}");

			ambientList.push_back(ambient);
			diffuseList.push_back(diffuse);
			shineStrengthList.push_back(shineStrength);
			shineList.push_back(shine);
			specularList.push_back(specular);
			texFileNameList.push_back(texFileName);

		}
		fscanStr(fp,"}");

		fscanStr(fp,"*GEOMOBJECT");
		fscanStr(fp,"{");
		fscanStr(fp,"*NODE_NAME"); string nodeName=fscanOneQuotation(fp);
		fscanStr(fp,"*NODE_TM");
		fscanStr(fp,"{");
		fscanStr(fp,"*NODE_NAME"); fscanOneQuotation(fp);
		fscanStr(fp,"*INHERIT_POS"); fscanVector3(fp);
		fscanStr(fp,"*INHERIT_ROT"); fscanVector3(fp);
		fscanStr(fp,"*INHERIT_SCL"); fscanVector3(fp);
		fscanStr(fp,"*TM_ROW0"); fscanVector3(fp);
		fscanStr(fp,"*TM_ROW1"); fscanVector3(fp);
		fscanStr(fp,"*TM_ROW2"); fscanVector3(fp);
		fscanStr(fp,"*TM_ROW3"); fscanVector3(fp);
		fscanStr(fp,"*TM_POS"); fscanVector3(fp);
		fscanStr(fp,"*TM_ROTAXIS"); fscanVector3(fp);
		fscanStr(fp,"*TM_ROTANGLE"); fscanFloat(fp);
		fscanStr(fp,"*TM_SCALE"); fscanVector3(fp);
		fscanStr(fp,"*TM_SCALEAXIS"); fscanVector3(fp);
		fscanStr(fp,"*TM_SCALEAXISANG"); fscanFloat(fp);


		fscanStr(fp,"}");
		fscanStr(fp,"*MESH");
		fscanStr(fp,"{");
		fscanStr(fp,"*TIMEVALUE"); fscanInt(fp);
		fscanStr(fp,"*MESH_NUMVERTEX"); int vertexCount=fscanInt(fp);
		fscanStr(fp,"*MESH_NUMFACES"); int faceCount=fscanInt(fp);
		fscanStr(fp,"*MESH_VERTEX_LIST");
		fscanStr(fp,"{");
		for(int vertexIndex=0;vertexIndex<vertexCount;vertexIndex++){
			fscanStr(fp,"*MESH_VERTEX");    int vID=fscanInt(fp);	Cc3dVector4 pos=fscanVector3(fp); pos.setw(1);
			assert(vertexIndex==vID);
			posList.push_back(pos);
		}
		fscanStr(fp,"}");

		fscanStr(fp,"*MESH_FACE_LIST");
		fscanStr(fp,"{");
		for(int faceIndex=0;faceIndex<faceCount;faceIndex++){

			fscanStr(fp,"*MESH_FACE");    
			string faceIDStr=fscanStr(fp); 
			int faceID=strToNumber(faceIDStr.substr(0,(int)faceIDStr.size()-1));
			fscanStr(fp,"A:"); int vIDA=fscanInt(fp);
			fscanStr(fp,"B:"); int vIDB=fscanInt(fp);
			fscanStr(fp,"C:"); int vIDC=fscanInt(fp);
			fscanStr(fp,"AB:"); fscanInt(fp);
			fscanStr(fp,"BC:"); fscanInt(fp);
			fscanStr(fp,"CA:"); fscanInt(fp);
			fscanStr(fp,"*MESH_SMOOTHING"); fscanInt(fp);
			fscanStr(fp,"*MESH_MTLID"); fscanInt(fp);

			Cc3dIDTriangle face(vIDA,vIDB,vIDC);
			faceList.push_back(face);

		}
		fscanStr(fp,"}");
		fscanStr(fp,"*MESH_NUMTVERTEX"); int tvertexCount=fscanInt(fp);
		fscanStr(fp,"*MESH_TVERTLIST");
		fscanStr(fp,"{");
		for(int tvertexIndex=0;tvertexIndex<tvertexCount;tvertexIndex++){
			fscanStr(fp,"*MESH_TVERT"); int tvertexID=fscanInt(fp);	Cc3dVector4 texCoord=fscanVector3(fp);
			assert(tvertexIndex==tvertexID);
			tvertexList.push_back(Cc3dVector2(texCoord.x(),texCoord.y()));
		}
		fscanStr(fp,"}");
		fscanStr(fp,"*MESH_NUMTVFACES"); int tfaceCount=fscanInt(fp);
		fscanStr(fp,"*MESH_TFACELIST");
		fscanStr(fp,"{");
		for(int tfaceIndex=0;tfaceIndex<tfaceCount;tfaceIndex++){
			fscanStr(fp,"*MESH_TFACE"); 
			int tfaceID=fscanInt(fp); 	
			assert(tfaceIndex==tfaceID);
			int tvID0=fscanInt(fp);
			int tvID1=fscanInt(fp);
			int tvID2=fscanInt(fp);
			Cc3dIDTriangle tface(tvID0,tvID1,tvID2);
			tfaceList.push_back(tface);
		}
		fscanStr(fp,"}");
		fscanStr(fp,"*MESH_NUMCVERTEX"); int cvertexCount=fscanInt(fp);
		if(cvertexCount!=0){
			//currently we only support cvertexCount==0 !
			assert(false);
		}
		fscanStr(fp,"*MESH_NORMALS");
		fscanStr(fp,"{");
		for(int faceIndex=0;faceIndex<faceCount;faceIndex++){
			fscanStr(fp,"*MESH_FACENORMAL"); int faceID=fscanInt(fp);	Cc3dVector4 faceNorm=fscanVector3(fp);	faceNorm.setw(0);
			assert(faceIndex==faceID);
			fscanStr(fp,"*MESH_VERTEXNORMAL"); int vID0=fscanInt(fp);	Cc3dVector4 norm0=fscanVector3(fp);  norm0.setw(0);
			fscanStr(fp,"*MESH_VERTEXNORMAL"); int vID1=fscanInt(fp);	Cc3dVector4 norm1=fscanVector3(fp);  norm1.setw(0);
			fscanStr(fp,"*MESH_VERTEXNORMAL"); int vID2=fscanInt(fp);	Cc3dVector4 norm2=fscanVector3(fp);  norm2.setw(0);
			faceNormList.push_back(faceNorm);
			faceV0NormList.push_back(norm0);
			faceV1NormList.push_back(norm1);
			faceV2NormList.push_back(norm2);
		}
		fscanStr(fp,"}");
		fscanStr(fp,"}");
		fscanStr(fp,"*PROP_MOTIONBLUR"); fscanInt(fp);
		fscanStr(fp,"*PROP_CASTSHADOW"); fscanInt(fp);
		fscanStr(fp,"*PROP_RECVSHADOW"); fscanInt(fp);
		fscanStr(fp,"*MATERIAL_REF"); materialRef=fscanInt(fp);
		fscanStr(fp,"}");

	}
	//convert data format and fill data to mesh
	{
		//----material and texture
		const int materialIDBase=(int)m_materialList.size();
		const int textureIDBase=(int)m_textureList.size();
		const int materialCount=(int)ambientList.size();
		for(int materialIndex=0;materialIndex<materialCount;materialIndex++){
			Cc3dMaterial*material=new Cc3dMaterial();
			material->m_ambient=ambientList[materialIndex];
			material->m_diffuse=diffuseList[materialIndex];
			material->m_shininess=shineStrengthList[materialIndex]/shineList[materialIndex];//i guess it like that, not sure!!!
			material->m_specular=specularList[materialIndex];
			m_materialList.push_back(material);
			Ctexture*texture=new Ctexture();
			string texFilePath=folderPath+"/"+texFileNameList[materialIndex];
			bool initTexSucc=texture->initWithFile(texFilePath);
			assert(initTexSucc);
			m_textureList.push_back(texture);

		}
		
		
		
		//----convert mesh
		vector<Cvert> vList;
		vector<Cc3dIDTriangle> IDtriList;
		int faceCount=(int)faceList.size();
		for(int faceIndex=0;faceIndex<faceCount;faceIndex++){
			const Cc3dIDTriangle&face=faceList[faceIndex];
			const Cc3dIDTriangle&tface=tfaceList[faceIndex];
			for(int i=0;i<3;i++){
				const int vID=face.vID(i);
				const int tvID=tface.vID(i);
				Cvert v;
				v.m_pos=posList[vID];
				v.m_norm;
				if(i==0){
					v.m_norm=faceV0NormList[faceIndex];
				}else if(i==1){
					v.m_norm=faceV1NormList[faceIndex];
				}else if(i==2){
					v.m_norm=faceV2NormList[faceIndex];
				}
				v.m_materialID=materialIDBase+materialRef;
				v.m_textureID=textureIDBase+materialRef;
				v.m_texCoord=tvertexList[tvID];
				vList.push_back(v);
			}
			int newVID0=faceIndex*3+0;
			int newVID1=faceIndex*3+1;
			int newVID2=faceIndex*3+2;
			IDtriList.push_back(Cc3dIDTriangle(newVID0,newVID1,newVID2));
		}//got vList and IDtriList
		//fill vList and IDtriList to mesh
		mesh->m_IDtriList=IDtriList;
		for(int i=0;i<(int)vList.size();i++){
			mesh->pushBackVert(vList[i]);
		}//got mesh

	}





	//--------------------------close file
	fclose(fp);
}
void Cgdi3dScene::loadModelMyFormat(const string&folderPath,const string&fileName,Cmesh*mesh){

	assert(mesh);
	assert(mesh->m_positionList.empty());

	vector<Ctexture*>&textureList=m_textureList;
	vector<Cc3dMaterial*>&materialList=m_materialList;

	//------------------full path of file
	string fileFullPath=folderPath+"/"+fileName;
	//open file
	FILE  *fp=NULL;
	fp=fopen(fileFullPath.c_str(),"r");// write
	if(fp==NULL){
		cout<<"Open "<<fileFullPath<<" failed!"<<endl;
		assert(false);
	}
	//------------------read file

	vector<Cmesh*> subMeshList;
	fscanStr(fp,"m_subMeshList.size");
	const int subMeshCount=fscanFloat(fp);
	for(int subMeshIndex=0;subMeshIndex<subMeshCount;subMeshIndex++){
		Cmesh* submesh=new Cmesh();
		Ctexture* texture=new Ctexture();
		Cc3dMaterial* material=new Cc3dMaterial();
		fscanStr(fp,"submeshID");
		int submeshID=fscanFloat(fp);
		assert(submeshID==subMeshIndex);
		fscanStr(fp,"{");
		{
			fscanStr(fp,"cullBack");
			int isCullBack=fscanInt(fp);
			fscanStr(fp,"RTmat");
			Cc3dVector4 col0=fscanVector4(fp);
			Cc3dVector4 col1=fscanVector4(fp);
			Cc3dVector4 col2=fscanVector4(fp);
			Cc3dVector4 col3=fscanVector4(fp);
			Cc3dMatrix4 RTmat(col0.x(),col0.y(),col0.z(),col0.w(),
				col1.x(),col1.y(),col1.z(),col1.w(),
				col2.x(),col2.y(),col2.z(),col2.w(),
				col3.x(),col3.y(),col3.z(),col3.w());
			Cc3dMatrix4 normMat=transpose(inverse(RTmat));
			fscanStr(fp,"texFilePath");
			string texFileName=fscanStr(fp);
			string texFilePath=folderPath+"/"+texFileName;
			//create texture
			bool initTexSucc=texture->initWithFile(texFilePath);
			assert(initTexSucc);
			fscanStr(fp,"ambient");
			Cc3dVector4 ambient=fscanVector4(fp);
			fscanStr(fp,"diffuse"); 
			Cc3dVector4 diffuse=fscanVector4(fp);
			fscanStr(fp,"specular"); 
			Cc3dVector4 specular=fscanVector4(fp);
			fscanStr(fp,"shininess");
			float shininess=fscanFloat(fp);
			//set to material
			material->m_ambient=ambient;
			material->m_diffuse=diffuse;
			material->m_specular=specular;
			material->m_shininess=shininess;
			fscanStr(fp,"m_vlist.size");
			const int vertexCount=fscanInt(fp);
			submesh->resizeVerts(vertexCount);//resize verts for submesh
			for(int vertexIndex=0;vertexIndex<vertexCount;vertexIndex++){
				Cvert v;
				fscanStr(fp,"vertex");
				//pos
				fscanStr(fp,"pos");
				v.m_pos=fscanVector3(fp);
				v.m_pos.setw(1);
				v.m_pos=RTmat*v.m_pos;
				//texCoord
				fscanStr(fp,"texCoord");
				v.m_texCoord=fscanVector2(fp);
				//norm
				fscanStr(fp,"norm");
				v.m_norm=fscanVector3(fp);
				v.m_norm.setw(0);
				v.m_norm=normMat*v.m_norm;
				//color
				fscanStr(fp,"color");
				v.m_color=fscanVector4(fp);
				//textureID and materialID
				v.m_textureID=(int)textureList.size();//subMeshIndex;
				v.m_materialID=(int)materialList.size();//subMeshIndex;
				v.m_isCullBack=isCullBack;
				//add to submesh
				submesh->setVert(vertexIndex,v);
			}
			fscanStr(fp,"m_IDtriList.size");
			const int IDtriCount=fscanInt(fp);
			submesh->resizeIDtris(IDtriCount);//resize IDtris for submesh
			for(int IDtriIndex=0;IDtriIndex<IDtriCount;IDtriIndex++){
				fscanStr(fp,"IDtri");
				int ID0=fscanInt(fp);
				int ID1=fscanInt(fp);
				int ID2=fscanInt(fp);
				Cc3dIDTriangle IDtri(ID0,ID1,ID2);
				//add to submesh
				submesh->m_IDtriList[IDtriIndex]=IDtri;
			}
		}
		fscanStr(fp,"}");
		//add to subMeshList, textureList, materialList
		subMeshList.push_back(submesh);
		textureList.push_back(texture);
		materialList.push_back(material);
	}//got subMeshList, textureList, materialList
	//merge subMeshes in subMeshList into mesh
	{
		int vertexCountSum=0;//sum of all subMesh's vertexCount
		int IDtriCountSum=0;//sum of all subMesh's IDtriCount
		const int subMeshCount=(int)subMeshList.size();
		for(int subMeshIndex=0;subMeshIndex<subMeshCount;subMeshIndex++){
			Cmesh*subMesh=subMeshList[subMeshIndex];
			const int vertexCount=(int)subMesh->m_positionList.size();
			const int IDtriCount=(int)subMesh->m_IDtriList.size();
			vertexCountSum+=vertexCount;
			IDtriCountSum+=IDtriCount;
		}//got vertexCountSum and IDtriCountSum
		//resize mesh
		mesh->resizeVerts(vertexCountSum);
		mesh->resizeIDtris(IDtriCountSum);
		//merge
		int curMeshVertID=0;
		int curMeshIDtriID=0;
		int previousSubMeshVertexCountSum=0;
		for(int subMeshIndex=0;subMeshIndex<subMeshCount;subMeshIndex++){
			Cmesh*subMesh=subMeshList[subMeshIndex];
			const int vertexCount=(int)subMesh->m_positionList.size();
			for(int vertexIndex=0;vertexIndex<vertexCount;vertexIndex++){
				Cvert v=subMesh->getVert(vertexIndex);
				mesh->setVert(curMeshVertID,v);
				curMeshVertID++;
			}
			const int IDtriCount=(int)subMesh->m_IDtriList.size();
			for(int IDtriIndex=0;IDtriIndex<IDtriCount;IDtriIndex++){
				Cc3dIDTriangle IDtri=subMesh->m_IDtriList[IDtriIndex];
				Cc3dIDTriangle IDtri_offseted(IDtri.vID(0)+previousSubMeshVertexCountSum,
											  IDtri.vID(1)+previousSubMeshVertexCountSum,
											  IDtri.vID(2)+previousSubMeshVertexCountSum);
				mesh->m_IDtriList[curMeshIDtriID]=IDtri_offseted;
				curMeshIDtriID++;
			}
			previousSubMeshVertexCountSum+=vertexCount;
		}//got mesh
		//release subMeshList
		for(int i=0;i<subMeshCount;i++){
			delete subMeshList[i];
		}subMeshList.clear();
	}//got mesh
	
	//--------------------------close file
	fclose(fp);

}
Cvert Cgdi3dScene::interpolateInLine_inViewportSpace(const Cvert&v1,const Cvert&v2,float x,float y){
	const float x1=v1.m_pos.x();
	const float y1=v1.m_pos.y();
	const float z1=v1.m_pos.z();
	const float w1=v1.m_pos.w();
	const float s1=v1.m_texCoord.x();
	const float t1=v1.m_texCoord.y();
	const float x2=v2.m_pos.x();
	const float y2=v2.m_pos.y();
	const float z2=v2.m_pos.z();
	const float w2=v2.m_pos.w();
	const float s2=v2.m_texCoord.x();
	const float t2=v2.m_texCoord.y();
	//----calculate interpolate factor k
	const float dx=v1.m_pos.x()-v2.m_pos.x();
	const float dy=v1.m_pos.y()-v2.m_pos.y();
	float k;
	if(abs(dx)>abs(dy)){
		//calculate k using xHighSubxLow
		k=(x-x2)/dx;
	}else{
		//calculate k using yHighSubyLow
		k=(y-y2)/dy;
	}
	//----calculate z
	float z=k*(z1-z2)+z2;
	//----calculate color
	Cc3dVector4 color=(v1.m_color-v2.m_color)*k+v2.m_color;
	//----calculate colorAdd
	Cc3dVector4 colorAdd=(v1.m_colorAdd-v2.m_colorAdd)*k+v2.m_colorAdd;
	//----calculate s,t
	const float f=m_camera->getzFar();
	const float n=m_camera->getzNear();
	const float M=(f==n)?M_INF:f/(f-n);
	const float s2_mul_z2SubM=s2*(z2-M);
	const float s1_mul_z1SubM=s1*(z1-M);
	const float t2_mul_z2SubM=t2*(z2-M);
	const float t1_mul_z1SubM=t1*(z1-M);
	float s_mul_zSubM=(s1_mul_z1SubM-s2_mul_z2SubM)*k+s2_mul_z2SubM;
	float t_mul_zSubM=(t1_mul_z1SubM-t2_mul_z2SubM)*k+t2_mul_z2SubM;
	float s=s_mul_zSubM/(z-M);
	float t=t_mul_zSubM/(z-M);
	//----calculate w
	float w=k*(w1-w2)+w2;
	//----set to v
	Cvert v=v1;//copy v1
	v.m_pos=cc3dv4(x,y,z,w);
	v.m_color=color;
	v.m_texCoord=Cc3dVector2(s,t);
	v.m_colorAdd=colorAdd;
	return v;

}

Cvert Cgdi3dScene::interpolateInLine_inClippingSpace(const Cvert&v1,const Cvert&v2,float x,float y){
	const float x1=v1.m_pos.x();
	const float y1=v1.m_pos.y();
	const float z1=v1.m_pos.z();
	const float w1=v1.m_pos.w();
	const float s1=v1.m_texCoord.x();
	const float t1=v1.m_texCoord.y();
	const float x2=v2.m_pos.x();
	const float y2=v2.m_pos.y();
	const float z2=v2.m_pos.z();
	const float w2=v2.m_pos.w();
	const float s2=v2.m_texCoord.x();
	const float t2=v2.m_texCoord.y();
	//----calculate interpolate factor k
	const float dx=v1.m_pos.x()-v2.m_pos.x();
	const float dy=v1.m_pos.y()-v2.m_pos.y();
	float k;
	if(abs(dx)>abs(dy)){
		//calculate k using xHighSubxLow
		k=(x-x2)/dx;
	}else{
		//calculate k using yHighSubyLow
		k=(y-y2)/dy;
	}
	//----calculate z
	float z=k*(z1-z2)+z2;
	//----calculate color
	Cc3dVector4 color=(v1.m_color-v2.m_color)*k+v2.m_color;
	//----calculate colorAdd
	Cc3dVector4 colorAdd=(v1.m_colorAdd-v2.m_colorAdd)*k+v2.m_colorAdd;
	//----calculate s,t
	//in clipping space s,t is linear with (x,y)
	float s=(s1-s2)*k+s2;
	float t=(t1-t2)*k+t2;
	//----calculate w
	float w=k*(w1-w2)+w2;
	//----set to v
	Cvert v=v1;//copy v1
	v.m_pos=cc3dv4(x,y,z,w);
	v.m_color=color;
	v.m_texCoord=Cc3dVector2(s,t);
	v.m_colorAdd=colorAdd;
	return v;

}
void Cgdi3dScene::clearZBuffer(){
	const float clearValue=1;
	int zBufferSize=(int)m_viewport.z()*(int)m_viewport.w();
	for(int i=0;i<zBufferSize;i++){
		m_zBuffer[i]=clearValue;
	}
}
void Cgdi3dScene::softRasterization(HDC hdc){
	
		
	submitMesh();
	transformVertexes();
	switch(m_drawMode){
	case eDrawSolid:
		rasterizeTriangles_solid(hdc);
		break;
	case eDrawEdge:
		rasterizeTriangles_edge(hdc);
		break;
	default:
		assert(false);
		break;
	}

	
}

void Cgdi3dScene::doClippingInClippingSpace(){
	//triangle reject
	roughTriRejectInClippingSpace();
	//clip against near plane
	clipAgainstNearPlaneInClippingSpace();
	//clip against far plane
	clipAgainstFarPlaneInClippingSpace();
	//clip against Left plane
	clipAgainstLeftPlaneInClippingSpace();
	//clip against Right plane
	clipAgainstRightPlaneInClippingSpace();
	//clip against Top plane
	clipAgainstTopPlaneInClippingSpace();
	//clip against Bottom plane
	clipAgainstBottomPlaneInClippingSpace();

	
}
void Cgdi3dScene::roughTriRejectInClippingSpace(){
	const int nIDtri=(int)m_transformedMesh->m_IDtriList.size();
	for(int IDtriIndex=0;IDtriIndex<nIDtri;IDtriIndex++){
		Cc3dIDTriangle&IDtri=m_transformedMesh->m_IDtriList[IDtriIndex];
		const int vID0=IDtri.vID(0);
		const int vID1=IDtri.vID(1);
		const int vID2=IDtri.vID(2);
		if(vID0==-1)continue;
		Cvert v0,v1,v2;
		v0=m_transformedMesh->getVert(vID0);
		v1=m_transformedMesh->getVert(vID1);
		v2=m_transformedMesh->getVert(vID2);
		//----mark the triangles which is must out of clipping space (some triangles may be out of clipping space but remain)
		if(isTriMustBeOutOfClippingSpace(v0.m_pos,v1.m_pos,v2.m_pos)){
			IDtri.init(-1,-1,-1);//invalidate this IDtri	
		}
	}
}
void Cgdi3dScene::clipAgainstNearPlaneInClippingSpace(){
	const int nIDtri=(int)m_transformedMesh->m_IDtriList.size();
	for(int IDtriIndex=0;IDtriIndex<nIDtri;IDtriIndex++){
		Cc3dIDTriangle&IDtri=m_transformedMesh->m_IDtriList[IDtriIndex];
		const int vID0=IDtri.vID(0);
		const int vID1=IDtri.vID(1);
		const int vID2=IDtri.vID(2);
		if(vID0==-1)continue;
		Cvert v0,v1,v2;
		v0=m_transformedMesh->getVert(vID0);
		v1=m_transformedMesh->getVert(vID1);
		v2=m_transformedMesh->getVert(vID2);
		//----cut triangle v0,v1,v2 with Near Plane, and throw the out of clipping space parts
		{
			vector<Ctri> triList=cutTriWithNearPlaneInClippingSpace(v0,v1,v2,-1);
			if(triList.empty()){//no cut occured
				//do nothing
			}else{//cut occured
				IDtri.init(-1,-1,-1);//invalidate the cutted IDtri	
				//add splited tris to m_transformedMesh
				const int triCount=(int)triList.size();
				for(int triIndex=0;triIndex<triCount;triIndex++){
					const Ctri&tri=triList[triIndex];
					m_transformedMesh->pushBackVert(tri.m_v0);int vID0=m_transformedMesh->m_positionList.size()-1;
					m_transformedMesh->pushBackVert(tri.m_v1);int vID1=m_transformedMesh->m_positionList.size()-1;
					m_transformedMesh->pushBackVert(tri.m_v2);int vID2=m_transformedMesh->m_positionList.size()-1;
					m_transformedMesh->m_IDtriList.push_back(Cc3dIDTriangle(vID0,vID1,vID2));
				}
			}//note: triangle count changed!
			continue;
		}
	}
}
void Cgdi3dScene::clipAgainstFarPlaneInClippingSpace(){
	const int nIDtri=(int)m_transformedMesh->m_IDtriList.size();
	for(int IDtriIndex=0;IDtriIndex<nIDtri;IDtriIndex++){
		Cc3dIDTriangle&IDtri=m_transformedMesh->m_IDtriList[IDtriIndex];
		const int vID0=IDtri.vID(0);
		const int vID1=IDtri.vID(1);
		const int vID2=IDtri.vID(2);
		if(vID0==-1)continue;
		Cvert v0,v1,v2;
		v0=m_transformedMesh->getVert(vID0);
		v1=m_transformedMesh->getVert(vID1);
		v2=m_transformedMesh->getVert(vID2);
		//----cut triangle v0,v1,v2 with Far Plane, and throw the out of clipping space parts
		{
			vector<Ctri> triList=cutTriWithFarPlaneInClippingSpace(v0,v1,v2,1);
			if(triList.empty()){//no cut occured
				//do nothing
			}else{//cut occured
				IDtri.init(-1,-1,-1);//invalidate the cutted IDtri	
				//add splited tris to m_transformedMesh
				const int triCount=(int)triList.size();
				for(int triIndex=0;triIndex<triCount;triIndex++){
					const Ctri&tri=triList[triIndex];
					m_transformedMesh->pushBackVert(tri.m_v0);int vID0=m_transformedMesh->m_positionList.size()-1;
					m_transformedMesh->pushBackVert(tri.m_v1);int vID1=m_transformedMesh->m_positionList.size()-1;
					m_transformedMesh->pushBackVert(tri.m_v2);int vID2=m_transformedMesh->m_positionList.size()-1;
					m_transformedMesh->m_IDtriList.push_back(Cc3dIDTriangle(vID0,vID1,vID2));
				}
			}
			//note: triangle count changed!
			continue;
		}
	}
}
void Cgdi3dScene::clipAgainstLeftPlaneInClippingSpace(){
	const int nIDtri=(int)m_transformedMesh->m_IDtriList.size();
	for(int IDtriIndex=0;IDtriIndex<nIDtri;IDtriIndex++){
		Cc3dIDTriangle&IDtri=m_transformedMesh->m_IDtriList[IDtriIndex];
		const int vID0=IDtri.vID(0);
		const int vID1=IDtri.vID(1);
		const int vID2=IDtri.vID(2);
		if(vID0==-1)continue;
		Cvert v0,v1,v2;
		v0=m_transformedMesh->getVert(vID0);
		v1=m_transformedMesh->getVert(vID1);
		v2=m_transformedMesh->getVert(vID2);
		//----cut triangle v0,v1,v2 with Near Plane, and throw the out of clipping space parts
		{
			vector<Ctri> triList=cutTriWithLeftPlaneInClippingSpace(v0,v1,v2,-1);
			if(triList.empty()){//no cut occured
				//do nothing
			}else{//cut occured
				IDtri.init(-1,-1,-1);//invalidate the cutted IDtri	
				//add splited tris to m_transformedMesh
				const int triCount=(int)triList.size();
				for(int triIndex=0;triIndex<triCount;triIndex++){
					const Ctri&tri=triList[triIndex];
					m_transformedMesh->pushBackVert(tri.m_v0);int vID0=m_transformedMesh->m_positionList.size()-1;
					m_transformedMesh->pushBackVert(tri.m_v1);int vID1=m_transformedMesh->m_positionList.size()-1;
					m_transformedMesh->pushBackVert(tri.m_v2);int vID2=m_transformedMesh->m_positionList.size()-1;
					m_transformedMesh->m_IDtriList.push_back(Cc3dIDTriangle(vID0,vID1,vID2));
				}
			}//note: triangle count changed!
			continue;
		}
	}

}
void Cgdi3dScene::clipAgainstRightPlaneInClippingSpace(){
	const int nIDtri=(int)m_transformedMesh->m_IDtriList.size();
	for(int IDtriIndex=0;IDtriIndex<nIDtri;IDtriIndex++){
		Cc3dIDTriangle&IDtri=m_transformedMesh->m_IDtriList[IDtriIndex];
		const int vID0=IDtri.vID(0);
		const int vID1=IDtri.vID(1);
		const int vID2=IDtri.vID(2);
		if(vID0==-1)continue;
		Cvert v0,v1,v2;
		v0=m_transformedMesh->getVert(vID0);
		v1=m_transformedMesh->getVert(vID1);
		v2=m_transformedMesh->getVert(vID2);
		//----cut triangle v0,v1,v2 with Far Plane, and throw the out of clipping space parts
		{
			vector<Ctri> triList=cutTriWithRightPlaneInClippingSpace(v0,v1,v2,1);
			if(triList.empty()){//no cut occured
				//do nothing
			}else{//cut occured
				IDtri.init(-1,-1,-1);//invalidate the cutted IDtri	
				//add splited tris to m_transformedMesh
				const int triCount=(int)triList.size();
				for(int triIndex=0;triIndex<triCount;triIndex++){
					const Ctri&tri=triList[triIndex];
					m_transformedMesh->pushBackVert(tri.m_v0);int vID0=m_transformedMesh->m_positionList.size()-1;
					m_transformedMesh->pushBackVert(tri.m_v1);int vID1=m_transformedMesh->m_positionList.size()-1;
					m_transformedMesh->pushBackVert(tri.m_v2);int vID2=m_transformedMesh->m_positionList.size()-1;
					m_transformedMesh->m_IDtriList.push_back(Cc3dIDTriangle(vID0,vID1,vID2));
				}
			}
			//note: triangle count changed!
			continue;
		}
	}
}
void Cgdi3dScene::clipAgainstTopPlaneInClippingSpace(){
	const int nIDtri=(int)m_transformedMesh->m_IDtriList.size();
	for(int IDtriIndex=0;IDtriIndex<nIDtri;IDtriIndex++){
		Cc3dIDTriangle&IDtri=m_transformedMesh->m_IDtriList[IDtriIndex];
		const int vID0=IDtri.vID(0);
		const int vID1=IDtri.vID(1);
		const int vID2=IDtri.vID(2);
		if(vID0==-1)continue;
		Cvert v0,v1,v2;
		v0=m_transformedMesh->getVert(vID0);
		v1=m_transformedMesh->getVert(vID1);
		v2=m_transformedMesh->getVert(vID2);
		//----cut triangle v0,v1,v2 with Far Plane, and throw the out of clipping space parts
		{
			vector<Ctri> triList=cutTriWithTopPlaneInClippingSpace(v0,v1,v2,1);
			if(triList.empty()){//no cut occured
				//do nothing
			}else{//cut occured
				IDtri.init(-1,-1,-1);//invalidate the cutted IDtri	
				//add splited tris to m_transformedMesh
				const int triCount=(int)triList.size();
				for(int triIndex=0;triIndex<triCount;triIndex++){
					const Ctri&tri=triList[triIndex];
					m_transformedMesh->pushBackVert(tri.m_v0);int vID0=m_transformedMesh->m_positionList.size()-1;
					m_transformedMesh->pushBackVert(tri.m_v1);int vID1=m_transformedMesh->m_positionList.size()-1;
					m_transformedMesh->pushBackVert(tri.m_v2);int vID2=m_transformedMesh->m_positionList.size()-1;
					m_transformedMesh->m_IDtriList.push_back(Cc3dIDTriangle(vID0,vID1,vID2));
				}
			}
			//note: triangle count changed!
			continue;
		}
	}
}
void Cgdi3dScene::clipAgainstBottomPlaneInClippingSpace(){
	const int nIDtri=(int)m_transformedMesh->m_IDtriList.size();
	for(int IDtriIndex=0;IDtriIndex<nIDtri;IDtriIndex++){
		Cc3dIDTriangle&IDtri=m_transformedMesh->m_IDtriList[IDtriIndex];
		const int vID0=IDtri.vID(0);
		const int vID1=IDtri.vID(1);
		const int vID2=IDtri.vID(2);
		if(vID0==-1)continue;
		Cvert v0,v1,v2;
		v0=m_transformedMesh->getVert(vID0);
		v1=m_transformedMesh->getVert(vID1);
		v2=m_transformedMesh->getVert(vID2);
		//----cut triangle v0,v1,v2 with Near Plane, and throw the out of clipping space parts
		{
			vector<Ctri> triList=cutTriWithBottomPlaneInClippingSpace(v0,v1,v2,-1);
			if(triList.empty()){//no cut occured
				//do nothing
			}else{//cut occured
				IDtri.init(-1,-1,-1);//invalidate the cutted IDtri	
				//add splited tris to m_transformedMesh
				const int triCount=(int)triList.size();
				for(int triIndex=0;triIndex<triCount;triIndex++){
					const Ctri&tri=triList[triIndex];
					m_transformedMesh->pushBackVert(tri.m_v0);int vID0=m_transformedMesh->m_positionList.size()-1;
					m_transformedMesh->pushBackVert(tri.m_v1);int vID1=m_transformedMesh->m_positionList.size()-1;
					m_transformedMesh->pushBackVert(tri.m_v2);int vID2=m_transformedMesh->m_positionList.size()-1;
					m_transformedMesh->m_IDtriList.push_back(Cc3dIDTriangle(vID0,vID1,vID2));
				}
			}//note: triangle count changed!
			continue;
		}
	}
}
void Cgdi3dScene::submitMesh(){
	//submit m_mesh to m_transformedMesh
	*m_transformedMesh=*m_mesh;
}
void Cgdi3dScene::transformVertexes(){

	//transform m_transformedMesh to clipping space
	{
		int vertexCount=(int)m_transformedMesh->m_positionList.size();
		for(int i=0;i<vertexCount;i++){
			//----get v
			Cvert v=m_transformedMesh->getVert(i);
			//----calculate transformedV
			Cvert transformedV=vertexShaderProgram(m_modelMat,m_camera,m_lightSource,m_materialList[v.m_materialID],v);
			//now transformedV is in clipping space
			//----set transformedV back to transformedMesh
			m_transformedMesh->setVert(i,transformedV);

		}

	}
	
	//do clipping in clipping space
	doClippingInClippingSpace();//note: now vertexCount of m_transformedMesh is changed!

	//transform to viewport space
	{
		const int vertexCount=m_transformedMesh->m_positionList.size();

		for(int i=0;i<vertexCount;i++){
			m_transformedMesh->m_positionList[i].persDiv();//in normalized device space
			m_transformedMesh->m_positionList[i]=m_viewportMat*m_transformedMesh->m_positionList[i];//in viewport space

		}

	}

	
}
bool Cgdi3dScene::isTriTotallyLeftOutOfClippingSpace(const Cc3dVector4&p0,const Cc3dVector4&p1,const Cc3dVector4&p2){
	if(p0.x()<-p0.w()&&p1.x()<-p1.w()&&p2.x()<-p2.w()){
		return true;
	}else{
		return false;
	}
}
bool Cgdi3dScene::isTriTotallyRightOutOfClippingSpace(const Cc3dVector4&p0,const Cc3dVector4&p1,const Cc3dVector4&p2){
	if(p0.x()>p0.w()&&p1.x()>p1.w()&&p2.x()>p2.w()){
		return true;
	}else{
		return false;
	}
}
bool Cgdi3dScene::isTriTotallyTopOutOfClippingSpace(const Cc3dVector4&p0,const Cc3dVector4&p1,const Cc3dVector4&p2){
	if(p0.y()>p0.w()&&p1.y()>p1.w()&&p2.y()>p2.w()){
		return true;
	}else{
		return false;
	}
}
bool  Cgdi3dScene::isTriTotallyBottomOutOfClippingSpace(const Cc3dVector4&p0,const Cc3dVector4&p1,const Cc3dVector4&p2){
	if(p0.y()<-p0.w()&&p1.y()<-p1.w()&&p2.y()<-p2.w()){
		return true;
	}else{
		return false;
	}
}
bool Cgdi3dScene::isTriTotallyFarOutOfClippingSpace(const Cc3dVector4&p0,const Cc3dVector4&p1,const Cc3dVector4&p2){
	if(p0.z()>p0.w()&&p1.z()>p1.w()&&p2.z()>p2.w()){
		return true;
	}else{
		return false;
	}
}
bool Cgdi3dScene::isTriTotallyNearOutOfClippingSpace(const Cc3dVector4&p0,const Cc3dVector4&p1,const Cc3dVector4&p2){
	if(p0.z()<-p0.w()&&p1.z()<-p1.w()&&p2.z()<-p2.w()){
		return true;
	}else{
		return false;
	}
}
bool Cgdi3dScene::isTriMustBeOutOfClippingSpace(const Cc3dVector4&p0,const Cc3dVector4&p1,const Cc3dVector4&p2){
	if(isTriTotallyLeftOutOfClippingSpace(p0,p1,p2)
		||isTriTotallyRightOutOfClippingSpace(p0,p1,p2)
		||isTriTotallyTopOutOfClippingSpace(p0,p1,p2)
		||isTriTotallyBottomOutOfClippingSpace(p0,p1,p2)
		||isTriTotallyNearOutOfClippingSpace(p0,p1,p2)
		||isTriTotallyFarOutOfClippingSpace(p0,p1,p2)){
			return true;
	
	}else{
		return false;
	}
}

vector<Ctri> Cgdi3dScene::cutTriWithNearPlaneInClippingSpace(const Cvert&v0,const Cvert&v1,const Cvert&v2,float Near){
	vector<Ctri> triList;
	vector<Cvert> vList;
	vList.push_back(v0);
	vList.push_back(v1);
	vList.push_back(v2);//got vList
	vector<bool> bList;
	for(int i=0;i<3;i++){
		bool b=(vList[i].m_pos.z()/vList[i].m_pos.w()<Near);
		bList.push_back(b);
	}//got bList and sumb
	if(bList[0]==bList[1]&&bList[0]==bList[2]){//all on same side
		//not cut
		//return empty triList
		return triList;
	}
	moveBoolListToTrueTrueTrueOrFalseFalseFalseOrTrueFalseFalseOrFalseTrueTrueMeanwhileMoveTheVertListAtTheSameTime(bList,vList);
	//now vList is ( >=Near , <Near , <Near ) or ( <Near , >=Near , >=Near )
	const Cvert&Va=vList[0];
	const Cvert&Vb=vList[1];
	const Cvert&Vc=vList[2];
	const float tAB=(Near*Va.m_pos.w()-Va.m_pos.z())/((Vb.m_pos.z()-Va.m_pos.z())-Near*(Vb.m_pos.w()-Va.m_pos.w()));
	Cc3dVector4 intersectPosOnLineAB=Va.m_pos+(Vb.m_pos-Va.m_pos)*tAB;
	const float tAC=(Near*Va.m_pos.w()-Va.m_pos.z())/((Vc.m_pos.z()-Va.m_pos.z())-Near*(Vc.m_pos.w()-Va.m_pos.w()));
	Cc3dVector4 intersectPosOnLineAC=Va.m_pos+(Vc.m_pos-Va.m_pos)*tAC;
	Cvert intersectVertOnLineAB=interpolateInLine_inClippingSpace(Va,Vb,intersectPosOnLineAB.x(),intersectPosOnLineAB.y());
	Cvert intersectVertOnLineAC=interpolateInLine_inClippingSpace(Va,Vc,intersectPosOnLineAC.x(),intersectPosOnLineAC.y());
	if(bList[0]==false){//vList is ( >=Near , <Near , <Near)
		Ctri tri;
		tri.m_v0=Va;
		tri.m_v1=intersectVertOnLineAB;
		tri.m_v2=intersectVertOnLineAC;
		triList.push_back(tri);
		return triList;
	}else{//vList is ( <Near , >=Near , >=Near )
		Ctri tri1;
		tri1.m_v0=intersectVertOnLineAB;
		tri1.m_v1=Vb;
		tri1.m_v2=Vc;
		Ctri tri2;
		tri2.m_v0=Vc;
		tri2.m_v1=intersectVertOnLineAC;
		tri2.m_v2=intersectVertOnLineAB;
		triList.push_back(tri1);
		triList.push_back(tri2);
		return triList;
	}
}
vector<Ctri> Cgdi3dScene::cutTriWithRightPlaneInClippingSpace(const Cvert&v0,const Cvert&v1,const Cvert&v2,float Right){
	vector<Ctri> triList;
	vector<Cvert> vList;
	vList.push_back(v0);
	vList.push_back(v1);
	vList.push_back(v2);//got vList
	vector<bool> bList;
	for(int i=0;i<3;i++){
		bool b=(vList[i].m_pos.x()/vList[i].m_pos.w()<Right);
		bList.push_back(b);
	}//got bList and sumb
	if(bList[0]==bList[1]&&bList[0]==bList[2]){//all on same side
		//not cut
		//return empty triList
		return triList;
	}
	moveBoolListToTrueTrueTrueOrFalseFalseFalseOrTrueFalseFalseOrFalseTrueTrueMeanwhileMoveTheVertListAtTheSameTime(bList,vList);
	//now vList is ( >=Right , <Right , <Right ) or ( <Right , >=Right , >=Right )
	const Cvert&Va=vList[0];
	const Cvert&Vb=vList[1];
	const Cvert&Vc=vList[2];
	const float tAB=(Right*Va.m_pos.w()-Va.m_pos.x())/((Vb.m_pos.x()-Va.m_pos.x())-Right*(Vb.m_pos.w()-Va.m_pos.w()));
	Cc3dVector4 intersectPosOnLineAB=Va.m_pos+(Vb.m_pos-Va.m_pos)*tAB;
	const float tAC=(Right*Va.m_pos.w()-Va.m_pos.x())/((Vc.m_pos.x()-Va.m_pos.x())-Right*(Vc.m_pos.w()-Va.m_pos.w()));
	Cc3dVector4 intersectPosOnLineAC=Va.m_pos+(Vc.m_pos-Va.m_pos)*tAC;
	Cvert intersectVertOnLineAB=interpolateInLine_inClippingSpace(Va,Vb,intersectPosOnLineAB.x(),intersectPosOnLineAB.y());
	Cvert intersectVertOnLineAC=interpolateInLine_inClippingSpace(Va,Vc,intersectPosOnLineAC.x(),intersectPosOnLineAC.y());
	if(bList[0]==false){//vList is ( >=Right , <Right , <Right)
		Ctri tri1;
		tri1.m_v0=intersectVertOnLineAB;
		tri1.m_v1=Vb;
		tri1.m_v2=Vc;
		Ctri tri2;
		tri2.m_v0=Vc;
		tri2.m_v1=intersectVertOnLineAC;
		tri2.m_v2=intersectVertOnLineAB;
		triList.push_back(tri1);
		triList.push_back(tri2);
		return triList;
	}else{//vList is ( <Right , >=Right , >=Right )
		Ctri tri;
		tri.m_v0=Va;
		tri.m_v1=intersectVertOnLineAB;
		tri.m_v2=intersectVertOnLineAC;
		triList.push_back(tri);
		return triList;
	}


}
vector<Ctri> Cgdi3dScene::cutTriWithLeftPlaneInClippingSpace(const Cvert&v0,const Cvert&v1,const Cvert&v2,float Left){
	vector<Ctri> triList;
	vector<Cvert> vList;
	vList.push_back(v0);
	vList.push_back(v1);
	vList.push_back(v2);//got vList
	vector<bool> bList;
	for(int i=0;i<3;i++){
		bool b=(vList[i].m_pos.x()/vList[i].m_pos.w()<Left);
		bList.push_back(b);
	}//got bList and sumb
	if(bList[0]==bList[1]&&bList[0]==bList[2]){//all on same side
		//not cut
		//return empty triList
		return triList;
	}
	moveBoolListToTrueTrueTrueOrFalseFalseFalseOrTrueFalseFalseOrFalseTrueTrueMeanwhileMoveTheVertListAtTheSameTime(bList,vList);
	//now vList is ( >=Left , <Left , <Left ) or ( <Left , >=Left , >=Left )
	const Cvert&Va=vList[0];
	const Cvert&Vb=vList[1];
	const Cvert&Vc=vList[2];
	const float tAB=(Left*Va.m_pos.w()-Va.m_pos.x())/((Vb.m_pos.x()-Va.m_pos.x())-Left*(Vb.m_pos.w()-Va.m_pos.w()));
	Cc3dVector4 intersectPosOnLineAB=Va.m_pos+(Vb.m_pos-Va.m_pos)*tAB;
	const float tAC=(Left*Va.m_pos.w()-Va.m_pos.x())/((Vc.m_pos.x()-Va.m_pos.x())-Left*(Vc.m_pos.w()-Va.m_pos.w()));
	Cc3dVector4 intersectPosOnLineAC=Va.m_pos+(Vc.m_pos-Va.m_pos)*tAC;
	Cvert intersectVertOnLineAB=interpolateInLine_inClippingSpace(Va,Vb,intersectPosOnLineAB.x(),intersectPosOnLineAB.y());
	Cvert intersectVertOnLineAC=interpolateInLine_inClippingSpace(Va,Vc,intersectPosOnLineAC.x(),intersectPosOnLineAC.y());
	if(bList[0]==false){//vList is ( >=Left , <Left , <Left)
		Ctri tri;
		tri.m_v0=Va;
		tri.m_v1=intersectVertOnLineAB;
		tri.m_v2=intersectVertOnLineAC;
		triList.push_back(tri);
		return triList;
	}else{//vList is ( <Left , >=Left , >=Left )
		Ctri tri1;
		tri1.m_v0=intersectVertOnLineAB;
		tri1.m_v1=Vb;
		tri1.m_v2=Vc;
		Ctri tri2;
		tri2.m_v0=Vc;
		tri2.m_v1=intersectVertOnLineAC;
		tri2.m_v2=intersectVertOnLineAB;
		triList.push_back(tri1);
		triList.push_back(tri2);
		return triList;
	}
}

vector<Ctri> Cgdi3dScene::cutTriWithTopPlaneInClippingSpace(const Cvert&v0,const Cvert&v1,const Cvert&v2,float Top){
	vector<Ctri> triList;
	vector<Cvert> vList;
	vList.push_back(v0);
	vList.push_back(v1);
	vList.push_back(v2);//got vList
	vector<bool> bList;
	for(int i=0;i<3;i++){
		bool b=(vList[i].m_pos.y()/vList[i].m_pos.w()<Top);
		bList.push_back(b);
	}//got bList and sumb
	if(bList[0]==bList[1]&&bList[0]==bList[2]){//all on same side
		//not cut
		//return empty triList
		return triList;
	}
	moveBoolListToTrueTrueTrueOrFalseFalseFalseOrTrueFalseFalseOrFalseTrueTrueMeanwhileMoveTheVertListAtTheSameTime(bList,vList);
	//now vList is ( >=Top , <Top , <Top ) or ( <Top , >=Top , >=Top )
	const Cvert&Va=vList[0];
	const Cvert&Vb=vList[1];
	const Cvert&Vc=vList[2];
	const float tAB=(Top*Va.m_pos.w()-Va.m_pos.y())/((Vb.m_pos.y()-Va.m_pos.y())-Top*(Vb.m_pos.w()-Va.m_pos.w()));
	Cc3dVector4 intersectPosOnLineAB=Va.m_pos+(Vb.m_pos-Va.m_pos)*tAB;
	const float tAC=(Top*Va.m_pos.w()-Va.m_pos.y())/((Vc.m_pos.y()-Va.m_pos.y())-Top*(Vc.m_pos.w()-Va.m_pos.w()));
	Cc3dVector4 intersectPosOnLineAC=Va.m_pos+(Vc.m_pos-Va.m_pos)*tAC;
	Cvert intersectVertOnLineAB=interpolateInLine_inClippingSpace(Va,Vb,intersectPosOnLineAB.x(),intersectPosOnLineAB.y());
	Cvert intersectVertOnLineAC=interpolateInLine_inClippingSpace(Va,Vc,intersectPosOnLineAC.x(),intersectPosOnLineAC.y());
	if(bList[0]==false){//vList is ( >=Top , <Top , <Top)
		Ctri tri1;
		tri1.m_v0=intersectVertOnLineAB;
		tri1.m_v1=Vb;
		tri1.m_v2=Vc;
		Ctri tri2;
		tri2.m_v0=Vc;
		tri2.m_v1=intersectVertOnLineAC;
		tri2.m_v2=intersectVertOnLineAB;
		triList.push_back(tri1);
		triList.push_back(tri2);
		return triList;
	}else{//vList is ( <Top , >=Top , >=Top )
		Ctri tri;
		tri.m_v0=Va;
		tri.m_v1=intersectVertOnLineAB;
		tri.m_v2=intersectVertOnLineAC;
		triList.push_back(tri);
		return triList;
	}
}
vector<Ctri> Cgdi3dScene::cutTriWithBottomPlaneInClippingSpace(const Cvert&v0,const Cvert&v1,const Cvert&v2,float Bottom){
	vector<Ctri> triList;
	vector<Cvert> vList;
	vList.push_back(v0);
	vList.push_back(v1);
	vList.push_back(v2);//got vList
	vector<bool> bList;
	for(int i=0;i<3;i++){
		bool b=(vList[i].m_pos.y()/vList[i].m_pos.w()<Bottom);
		bList.push_back(b);
	}//got bList and sumb
	if(bList[0]==bList[1]&&bList[0]==bList[2]){//all on same side
		//not cut
		//return empty triList
		return triList;
	}
	moveBoolListToTrueTrueTrueOrFalseFalseFalseOrTrueFalseFalseOrFalseTrueTrueMeanwhileMoveTheVertListAtTheSameTime(bList,vList);
	//now vList is ( >=Bottom , <Bottom , <Bottom ) or ( <Bottom , >=Bottom , >=Bottom )
	const Cvert&Va=vList[0];
	const Cvert&Vb=vList[1];
	const Cvert&Vc=vList[2];
	const float tAB=(Bottom*Va.m_pos.w()-Va.m_pos.y())/((Vb.m_pos.y()-Va.m_pos.y())-Bottom*(Vb.m_pos.w()-Va.m_pos.w()));
	Cc3dVector4 intersectPosOnLineAB=Va.m_pos+(Vb.m_pos-Va.m_pos)*tAB;
	const float tAC=(Bottom*Va.m_pos.w()-Va.m_pos.y())/((Vc.m_pos.y()-Va.m_pos.y())-Bottom*(Vc.m_pos.w()-Va.m_pos.w()));
	Cc3dVector4 intersectPosOnLineAC=Va.m_pos+(Vc.m_pos-Va.m_pos)*tAC;
	Cvert intersectVertOnLineAB=interpolateInLine_inClippingSpace(Va,Vb,intersectPosOnLineAB.x(),intersectPosOnLineAB.y());
	Cvert intersectVertOnLineAC=interpolateInLine_inClippingSpace(Va,Vc,intersectPosOnLineAC.x(),intersectPosOnLineAC.y());
	if(bList[0]==false){//vList is ( >=Bottom , <Bottom , <Bottom)
		Ctri tri;
		tri.m_v0=Va;
		tri.m_v1=intersectVertOnLineAB;
		tri.m_v2=intersectVertOnLineAC;
		triList.push_back(tri);
		return triList;
	}else{//vList is ( <Bottom , >=Bottom , >=Bottom )
		Ctri tri1;
		tri1.m_v0=intersectVertOnLineAB;
		tri1.m_v1=Vb;
		tri1.m_v2=Vc;
		Ctri tri2;
		tri2.m_v0=Vc;
		tri2.m_v1=intersectVertOnLineAC;
		tri2.m_v2=intersectVertOnLineAB;
		triList.push_back(tri1);
		triList.push_back(tri2);
		return triList;
	}
}
vector<Ctri> Cgdi3dScene::cutTriWithFarPlaneInClippingSpace(const Cvert&v0,const Cvert&v1,const Cvert&v2,float Far){
	vector<Ctri> triList;
	vector<Cvert> vList;
	vList.push_back(v0);
	vList.push_back(v1);
	vList.push_back(v2);//got vList
	vector<bool> bList;
	for(int i=0;i<3;i++){
		bool b=(vList[i].m_pos.z()/vList[i].m_pos.w()<Far);
		bList.push_back(b);
	}//got bList and sumb
	if(bList[0]==bList[1]&&bList[0]==bList[2]){//all on same side
		//not cut
		//return empty triList
		return triList;
	}
	moveBoolListToTrueTrueTrueOrFalseFalseFalseOrTrueFalseFalseOrFalseTrueTrueMeanwhileMoveTheVertListAtTheSameTime(bList,vList);
	//now vList is ( >=Far , <Far , <Far ) or ( <Far , >=Far , >=Far )
	const Cvert&Va=vList[0];
	const Cvert&Vb=vList[1];
	const Cvert&Vc=vList[2];
	const float tAB=(Far*Va.m_pos.w()-Va.m_pos.z())/((Vb.m_pos.z()-Va.m_pos.z())-Far*(Vb.m_pos.w()-Va.m_pos.w()));
	Cc3dVector4 intersectPosOnLineAB=Va.m_pos+(Vb.m_pos-Va.m_pos)*tAB;
	const float tAC=(Far*Va.m_pos.w()-Va.m_pos.z())/((Vc.m_pos.z()-Va.m_pos.z())-Far*(Vc.m_pos.w()-Va.m_pos.w()));
	Cc3dVector4 intersectPosOnLineAC=Va.m_pos+(Vc.m_pos-Va.m_pos)*tAC;
	Cvert intersectVertOnLineAB=interpolateInLine_inClippingSpace(Va,Vb,intersectPosOnLineAB.x(),intersectPosOnLineAB.y());
	Cvert intersectVertOnLineAC=interpolateInLine_inClippingSpace(Va,Vc,intersectPosOnLineAC.x(),intersectPosOnLineAC.y());
	if(bList[0]==false){//vList is ( >=Far , <Far , <Far)
		Ctri tri1;
		tri1.m_v0=intersectVertOnLineAB;
		tri1.m_v1=Vb;
		tri1.m_v2=Vc;
		Ctri tri2;
		tri2.m_v0=Vc;
		tri2.m_v1=intersectVertOnLineAC;
		tri2.m_v2=intersectVertOnLineAB;
		triList.push_back(tri1);
		triList.push_back(tri2);
		return triList;
	}else{//vList is ( <Far , >=Far , >=Far )
		Ctri tri;
		tri.m_v0=Va;
		tri.m_v1=intersectVertOnLineAB;
		tri.m_v2=intersectVertOnLineAC;
		triList.push_back(tri);
		return triList;
	}




}


void Cgdi3dScene::rasterizeTriangles_solid(HDC hdc){
	//clear zBuffer
	clearZBuffer();
	//fill triangles edges
	int nIDtri=(int)m_transformedMesh->m_IDtriList.size();
	for(int i=0;i<nIDtri;i++){
		const Cc3dIDTriangle&IDtri=m_transformedMesh->m_IDtriList[i];
		const int vID0=IDtri.vID(0);
		const int vID1=IDtri.vID(1);
		const int vID2=IDtri.vID(2);
		if(vID0==-1)continue;
		Cvert v0,v1,v2;
		v0=m_transformedMesh->getVert(vID0);
		v1=m_transformedMesh->getVert(vID1);
		v2=m_transformedMesh->getVert(vID2);
		fillTriFace_solid(hdc,v0,v1,v2);
	}
}

void Cgdi3dScene::rasterizeTriangles_edge(HDC hdc){
	//clear zBuffer
	clearZBuffer();
	//fill triangles 
	{
		int nIDtri=(int)m_transformedMesh->m_IDtriList.size();
		for(int i=0;i<nIDtri;i++){
			const Cc3dIDTriangle&IDtri=m_transformedMesh->m_IDtriList[i];
			const int vID0=IDtri.vID(0);
			const int vID1=IDtri.vID(1);
			const int vID2=IDtri.vID(2);
			if(vID0==-1)continue;
			Cvert v0,v1,v2;
			v0=m_transformedMesh->getVert(vID0);
			v1=m_transformedMesh->getVert(vID1);
			v2=m_transformedMesh->getVert(vID2);

			double polygonOffset=0;
			{
				//----calculate polygonOffset
				//ref: https://www.khronos.org/opengles/sdk/docs/man/xhtml/glPolygonOffset.xml
				//ref: https://www.opengl.org/archives/resources/faq/technical/polygonoffset.htm
				//calculate z-gradient
				float gradZ=0;
				Cc3dVector4 p0p1=v1.m_pos-v0.m_pos;
				Cc3dVector4 p0p2=v2.m_pos-v0.m_pos;
				Cc3dVector4 normVec=cross(p0p1,p0p2);
				normVec.setw(0);
				if(getLength(normVec)<=0.0001){
					gradZ=0;
				}else{
					Cc3dVector4 negNormVec=normVec*(-1);
					Cc3dVector4 negZ=cc3dv4(0,0,-1,0);
					Cc3dVector4 crossNegNormVecNegZ=cross(negNormVec,negZ);
					Cc3dVector4 gradVec=cross(crossNegNormVecNegZ,negNormVec);
					gradVec.setw(0);

					Cc3dVector4 gradDir=normalize(gradVec);
					assert(gradDir.z()<=0);
					gradZ=abs(gradDir.z());
				}

				double DZ=gradZ;
				double units=m_polygonOffsetForEdgeMode_units;
				double factor=m_polygonOffsetForEdgeMode_factor;
				const double r=m_polygonOffsetForAllMode_r;
				polygonOffset=factor*DZ+r*units;
			}

			fillTriFace_depthOnly(hdc,v0,v1,v2,polygonOffset);
		}

	}


	//fill triangles edges
	{
		int nIDtri=(int)m_transformedMesh->m_IDtriList.size();
		for(int i=0;i<nIDtri;i++){
			const Cc3dIDTriangle&IDtri=m_transformedMesh->m_IDtriList[i];
			const int vID0=IDtri.vID(0);
			const int vID1=IDtri.vID(1);
			const int vID2=IDtri.vID(2);
			if(vID0==-1)continue;
			Cvert v0,v1,v2;
			v0=m_transformedMesh->getVert(vID0);
			v1=m_transformedMesh->getVert(vID1);
			v2=m_transformedMesh->getVert(vID2);
			fillTriFace_edge(hdc,v0,v1,v2);
		}
	}
}
bool Cgdi3dScene::thisViewportSpaceTriIsFacingUs(const Cc3dVector4&p0,const Cc3dVector4&p1,const Cc3dVector4&p2){
	Cc3dVector4 p0p1=p1-p0;
	Cc3dVector4 p0p2=p2-p0;
	Cc3dVector4 faceDir=cross(p0p1,p0p2)*(-1);//note: clipping space and viewport space is left hand space, so faceDir is cross(p0p1,p0p2)*(-1)
	if(faceDir.z()<0){//facing us
		return true;
	}else{
		return false;
	}
}



void Cgdi3dScene::drawPixel_OAtLD(HDC hdc,float x,float y,const Cc3dVector4&color){
	//note: before pass the color to SetPixel, we must clamp it, or will get to wrong result
	int r_clamp=max(0,min(1,color.r()))*255;
	int g_clamp=max(0,min(1,color.g()))*255;
	int b_clamp=max(0,min(1,color.b()))*255;
	SetPixel(hdc, x, m_viewport.w()-y, RGB(r_clamp,g_clamp,b_clamp));
}
void Cgdi3dScene::drawLine_OAtLD(HDC hdc,float x1,float y1,float x2,float y2){
	MoveToEx(hdc,x1,m_viewport.w()-y1,0);
	LineTo(hdc,x2,m_viewport.w()-y2);
}
void Cgdi3dScene::drawTriWire(HDC hdc,const Cc3dVector4&p0,const Cc3dVector4&p1,const Cc3dVector4&p2){
	//p0p1
	drawLine_OAtLD(hdc,p0.x(),p0.y(),p1.x(),p1.y());
	//p1p2
	drawLine_OAtLD(hdc,p1.x(),p1.y(),p2.x(),p2.y());
	//p2p0
	drawLine_OAtLD(hdc,p2.x(),p2.y(),p0.x(),p0.y());
}


Cc3dMatrix4 Cgdi3dScene::calculateViewportMat(Cc3dVector4 viewport){
	const float vx=viewport.x();
	const float vy=viewport.y();
	const float w=viewport.z();
	const float h=viewport.w();
	Cc3dMatrix4 viewportMat(w/2,0,0,0,//col1
		0,h/2,0,0,//col2
		0,0,0.5,0,//col3
		vx+w/2,vy+h/2,0.5,1);//col4
	return viewportMat;
}
double Cgdi3dScene::readZBuffer(int x,int y){
	const int w=(int)m_viewport.z();
	const int h=(int)m_viewport.w();
	if(x>=0&&x<w&&y>=0&&y<h){
		return m_zBuffer[y*w+x];
	}else{
		return 1;
	}

}
void Cgdi3dScene::writeZBuffer(int x,int y,double zValueToWrite){
	const int w=(int)m_viewport.z();
	const int h=(int)m_viewport.w();
	if(x>=0&&x<w&&y>=0&&y<h){
		m_zBuffer[y*w+x]=zValueToWrite;
	}
}
CearlyZOutput Cgdi3dScene::interpolateInTri_inViewportSpace_earlyZ(const Cvert&v0,const Cvert&v1,const Cvert&v2,float x,float y){
	const float x0=v0.m_pos.x();
	const float y0=v0.m_pos.y();
	const float z0=v0.m_pos.z();
	const float x1=v1.m_pos.x();
	const float y1=v1.m_pos.y();
	const float z1=v1.m_pos.z();
	const float x2=v2.m_pos.x();
	const float y2=v2.m_pos.y();
	const float z2=v2.m_pos.z();
	const float y1suby0=y1-y0;
	const float y2suby0=y2-y0;
	const float ysuby0=y-y0;
	const float x1subx0=x1-x0;
	const float x2subx0=x2-x0;
	const float xsubx0=x-x0;
	const float M=y1suby0*x2subx0-x1subx0*y2suby0;
	float A=0;
	float B=0;
	float z=1;
	if(M==0){//  p0p1//p0p2, p0p1p2 is degenerated tri, this may not happen, so no matter return what
		//do nothing
	}else{
		A=-(y2suby0*xsubx0-x2subx0*ysuby0)/M;
		B=(y1suby0*xsubx0-x1subx0*ysuby0)/M;
		//----calculate z
		z=z0+(z1-z0)*A+(z2-z0)*B;

	}
	CearlyZOutput earlyZOutput;
	earlyZOutput.m_A=A;
	earlyZOutput.m_B=B;
	earlyZOutput.m_x=x;
	earlyZOutput.m_y=y;
	earlyZOutput.m_z=z;
	return earlyZOutput;
}
Cvert Cgdi3dScene::interpolateInTri_inViewportSpace_otherAttributes(const Cvert&v0,const Cvert&v1,const Cvert&v2,const CearlyZOutput&earlyZOutput){
	const float A=earlyZOutput.m_A;
	const float B=earlyZOutput.m_B;
	const float z=earlyZOutput.m_z;
	const float x=earlyZOutput.m_x;
	const float y=earlyZOutput.m_y;
	const float z0=v0.m_pos.z();
	const float w0=v0.m_pos.w();
	const float s0=v0.m_texCoord.x();
	const float t0=v0.m_texCoord.y();
	const float z1=v1.m_pos.z();
	const float w1=v1.m_pos.w();
	const float s1=v1.m_texCoord.x();
	const float t1=v1.m_texCoord.y();
	const float z2=v2.m_pos.z();
	const float w2=v2.m_pos.w();
	const float s2=v2.m_texCoord.x();
	const float t2=v2.m_texCoord.y();
	//----calculate s,t
	//in viewport space, s*(z-(f+n)/(f-n))=Linear(x,y)
	float s,t;
	{
		const float f=m_camera->getzFar();
		const float n=m_camera->getzNear();
		const float M=(f==n)?M_INF:f/(f-n);
		const float s0_mul_z0SubM=s0*(z0-M);
		const float s1_mul_z1SubM=s1*(z1-M);
		const float s2_mul_z2SubM=s2*(z2-M);
		float s0_mul_zSubM=s0_mul_z0SubM+(s1_mul_z1SubM-s0_mul_z0SubM)*A+(s2_mul_z2SubM-s0_mul_z0SubM)*B;
		const float t0_mul_z0SubM=t0*(z0-M);
		const float t1_mul_z1SubM=t1*(z1-M);
		const float t2_mul_z2SubM=t2*(z2-M);
		float t_mul_zSubM=t0_mul_z0SubM+(t1_mul_z1SubM-t0_mul_z0SubM)*A+(t2_mul_z2SubM-t0_mul_z0SubM)*B;
		s=s0_mul_zSubM/(z-M);
		t=t_mul_zSubM/(z-M);
	}
	//----calculate w
	float w=w0+(w1-w0)*A+(w2-w0)*B;
	//----calculate color
	Cc3dVector4 color=v0.m_color+(v1.m_color-v0.m_color)*A+(v2.m_color-v0.m_color)*B;
	//----calculate colorAdd
	Cc3dVector4 colorAdd=v0.m_colorAdd+(v1.m_colorAdd-v0.m_colorAdd)*A+(v2.m_colorAdd-v0.m_colorAdd)*B;
	//----make v
	Cvert v=v0;//copy v0
	v.m_pos=cc3dv4(x,y,z,w);
	v.m_texCoord=Cc3dVector2(s,t);
	v.m_color=color;
	v.m_colorAdd=colorAdd;
	return v;
}







void Cgdi3dScene::fillTriFace_solid(HDC hdc,const Cvert&v0,const Cvert&v1,const Cvert&v2){
	//cull back face
	if(m_isCullBackFace){
		if(v0.m_isCullBack){
			if(thisViewportSpaceTriIsFacingUs(v0.m_pos,v1.m_pos,v2.m_pos)==false){
				return;
			}
		}
	}

	vector<Cvert> vlist;
	vlist.push_back(v0);
	vlist.push_back(v1);
	vlist.push_back(v2);
	vector<int> downOrderIndexList=getThreeValuesDownOrderIndexList(v0.m_pos.y(),v1.m_pos.y(),v2.m_pos.y());
	const Cvert&vHigh=vlist[downOrderIndexList[0]];
	const Cvert&vMid=vlist[downOrderIndexList[1]];
	const Cvert&vLow=vlist[downOrderIndexList[2]];
	const bool triIsPanTop=(vHigh.m_pos.y()==vMid.m_pos.y());
	const bool triIsPanBottom=(vMid.m_pos.y()==vLow.m_pos.y());
	if(triIsPanTop&&triIsPanBottom){//degenerated tri, actually a horizontal line
		//ignore it
	
	}else if(!triIsPanTop&&triIsPanBottom){//only pan bottom
		if(vLow.m_pos.x()==vMid.m_pos.x()){
			//degenerated tri, actually a line
			//ignore it
		}else if(vLow.m_pos.x()<vMid.m_pos.x()){
			//panBottomTri(vHigh,vLow,vMid)
			fillPanBottomTriFace_solid(hdc,vHigh,vLow,vMid);
		}else{
			//panBottomTri(vHigh,vMid,vLow)
			fillPanBottomTriFace_solid(hdc,vHigh,vMid,vLow);
		}


	}else if(triIsPanTop&&!triIsPanBottom){//only pan top
		if(vHigh.m_pos.x()==vMid.m_pos.x()){
			//degenerated tri, actually a line
			//ignore it
		}else if(vHigh.m_pos.x()<vMid.m_pos.x()){
			//panTopTri(vHigh,vLow,vMid)
			fillPanTopTriFace_solid(hdc,vHigh,vLow,vMid);
		}else{
			//panTopTri(vMid,vLow,vHigh)
			fillPanTopTriFace_solid(hdc,vMid,vLow,vHigh);
		}

	}else if(!triIsPanTop&&!triIsPanBottom){//not pan bottom and not pan top
		//a horizontal line across vMid intersect with line=(vHigh,vLow) in point v
		Cvert v=linePHighPLowIntersectWithHorizontalLineAcrossPMid(vHigh,vLow,vMid);
		v=interpolateInLine_inViewportSpace(vHigh,vLow,v.m_pos.x(),v.m_pos.y());
			
		if(v.m_pos.x()==vMid.m_pos.x()){
			//triangle is actually a nonHorizon line
			//ignore this degenerated tri
		}else if(v.m_pos.x()<vMid.m_pos.x()){
			//panBottomTri(vHigh,v,vMid) and panTopTri(v,vLow,vMid)
			fillPanBottomTriFace_solid(hdc,vHigh,v,vMid);
			fillPanTopTriFace_solid(hdc,v,vLow,vMid);
		}else{
			//panBottomTri(vHigh,vMid,v) and panTopTri(vMid,vLow,v)
			fillPanBottomTriFace_solid(hdc,vHigh,vMid,v);
			fillPanTopTriFace_solid(hdc,vMid,vLow,v);
	
		}
	}else{//impossible to reach this branch
		assert(false);
	}
	





}
void Cgdi3dScene::fillPanBottomTriFace_solid(HDC hdc,const Cvert&vTop,const Cvert&vBottomLeft,const Cvert&vBottomRight){
	//i use below Rasterization Rules:
	//	 xLeft_int=ceil(xLeft-0.5);
	//	 xRight_int=ceil(xRight-1.5);
	//	 yTop_int=ceil(yTop-1.5);
	//	 yBottom_int=ceil(yBottom-0.5);

	float yTop=vTop.m_pos.y();
	float yBottom=vBottomLeft.m_pos.y();
	int yTop_int,yBottom_int;
	{
		
		yTop_int=ceil(yTop-1.5);
		yBottom_int=ceil(yBottom-0.5);

	}
	
	const double leftDxDivDy=((double)vTop.m_pos.x()-(double)vBottomLeft.m_pos.x())/((double)vTop.m_pos.y()-(double)vBottomLeft.m_pos.y());
	const double rightDxDivDy=((double)vTop.m_pos.x()-(double)vBottomRight.m_pos.x())/((double)vTop.m_pos.y()-(double)vBottomRight.m_pos.y());
	double xLeft=lineIntersectWithHorizontalLine(vTop.m_pos,vBottomLeft.m_pos,yBottom_int+0.5);
	double xRight=lineIntersectWithHorizontalLine(vTop.m_pos,vBottomRight.m_pos,yBottom_int+0.5);
	
	for(int y_int=yBottom_int;y_int<=yTop_int;y_int++){
		int xLeft_int,xRight_int;
		{
			xLeft_int=ceil(xLeft-0.5);
			xRight_int=ceil(xRight-1.5);
		}

		for(int x_int=xLeft_int;x_int<=xRight_int;x_int++){
			const Cc3dVector2&pixelCenter=Cc3dVector2(x_int+0.5,y_int+0.5);//note: pixel(x_int,y_int)'s center is (x_int+0.5,y_int+0.5)
			CearlyZOutput earlyZOutput=interpolateInTri_inViewportSpace_earlyZ(vTop,vBottomLeft,vBottomRight,pixelCenter.x(),pixelCenter.y());
			float z_inbuffer=readZBuffer(x_int,y_int);
			if(earlyZOutput.m_z<z_inbuffer){

				Cvert interpolatedV=interpolateInTri_inViewportSpace_otherAttributes(vTop,vBottomLeft,vBottomRight,earlyZOutput);
				Cfrag frag=fragShaderProgram(interpolatedV,m_textureList[interpolatedV.m_textureID]);
				drawPixel_OAtLD(hdc,x_int,y_int,frag.m_color);

				writeZBuffer(x_int,y_int,earlyZOutput.m_z);



			}

		}


		xLeft+=leftDxDivDy;
		xRight+=rightDxDivDy;

		

	}



}

void Cgdi3dScene::fillPanTopTriFace_solid(HDC hdc,const Cvert&vTopLeft,const Cvert&vBottom,const Cvert&vTopRight){
	//i use below Rasterization Rules:
	//	 xLeft_int=ceil(xLeft-0.5);
	//	 xRight_int=ceil(xRight-1.5);
	//	 yTop_int=ceil(yTop-1.5);
	//	 yBottom_int=ceil(yBottom-0.5);

	float yTop=vTopLeft.m_pos.y();
	float yBottom=vBottom.m_pos.y();
    int yTop_int,yBottom_int;
	{
	
		yTop_int=ceil(yTop-1.5);
		yBottom_int=floor(yBottom-0.5);

	}
	const double leftDxDivDy=((double)vTopLeft.m_pos.x()-(double)vBottom.m_pos.x())/((double)vTopLeft.m_pos.y()-(double)vBottom.m_pos.y());
	const double rightDxDivDy=((double)vTopRight.m_pos.x()-(double)vBottom.m_pos.x())/((double)vTopRight.m_pos.y()-(double)vBottom.m_pos.y());
	double xLeft=lineIntersectWithHorizontalLine(vBottom.m_pos,vTopLeft.m_pos,yBottom_int+0.5);
	double xRight=lineIntersectWithHorizontalLine(vBottom.m_pos,vTopRight.m_pos,yBottom_int+0.5);


	for(int y_int=yBottom_int;y_int<=yTop_int;y_int++){
		int xLeft_int,xRight_int;
		{
			xLeft_int=ceil(xLeft-0.5);
			xRight_int=ceil(xRight-1.5);


		}

		for(int x_int=xLeft_int;x_int<=xRight_int;x_int++){
			const Cc3dVector2&pixelCenter=Cc3dVector2(x_int+0.5,y_int+0.5);//note: pixel(x_int,y_int)'s center is (x_int+0.5,y_int+0.5)
			CearlyZOutput earlyZOutput=interpolateInTri_inViewportSpace_earlyZ(vBottom,vTopLeft,vTopRight,pixelCenter.x(),pixelCenter.y());
			float z_inbuffer=readZBuffer(x_int,y_int);
			if(earlyZOutput.m_z<z_inbuffer){

				Cvert interpolatedV=interpolateInTri_inViewportSpace_otherAttributes(vBottom,vTopLeft,vTopRight,earlyZOutput);
				Cfrag frag=fragShaderProgram(interpolatedV,m_textureList[interpolatedV.m_textureID]);
				drawPixel_OAtLD(hdc,x_int,y_int,frag.m_color);


				writeZBuffer(x_int,y_int,earlyZOutput.m_z);

			}
		}




		xLeft+=leftDxDivDy;
		xRight+=rightDxDivDy;


	}

}



void Cgdi3dScene::fillTriFace_edge(HDC hdc,const Cvert&v0,const Cvert&v1,const Cvert&v2){
	//----cull back face
	if(m_isCullBackFace){
		if(v0.m_isCullBack){
			if(thisViewportSpaceTriIsFacingUs(v0.m_pos,v1.m_pos,v2.m_pos)==false){
				return;
			}
		}
		
	}


	//----draw tri(v0,v1,v2)
	fillLine(hdc,v0,v1);
	fillLine(hdc,v1,v2);
	fillLine(hdc,v2,v0);

}
void Cgdi3dScene::fillLine(HDC hdc,const Cvert&v0,const Cvert&v1){
	
	const float dx=v1.m_pos.x()-v0.m_pos.x();
	const float dy=v1.m_pos.y()-v0.m_pos.y();
	if(abs(dy)>abs(dx)){//x-majoy
		//scan row by row
		Cvert vHigh,vLow;
		if(v0.m_pos.y()>v1.m_pos.y()){
			vHigh=v0;
			vLow=v1;
		}else{
			vHigh=v1;
			vLow=v0;
		}//got vHigh,vLow

		int yTop_int,yBottom_int;
		{
			float yTop=vHigh.m_pos.y();
			float yBottom=vLow.m_pos.y();
			yTop_int=floor(yTop+0.5);
			yBottom_int=floor(yBottom+0.5);
		}	

		
		if(1){
			const double DxDivDy=((double)vHigh.m_pos.x()-(double)vLow.m_pos.x())/((double)vHigh.m_pos.y()-(double)vLow.m_pos.y());
			double y=yBottom_int+0.5;
			double x=(y-vLow.m_pos.y())*(vHigh.m_pos.x()-vLow.m_pos.x())/(vHigh.m_pos.y()-vLow.m_pos.y())+vLow.m_pos.x();
			for(int y_int=yBottom_int;y_int<=yTop_int;y_int++){
				int x_int;
				{
					x_int=floor(x+0.5);
				
				}
				


	
				const Cc3dVector2&pixelCenter=Cc3dVector2(x_int+0.5,y_int+0.5);//note: pixel(x_int,y_int)'s center is (x_int+0.5,y_int+0.5)
				Cvert interpolatedV=interpolateInLine_inViewportSpace(vLow,vHigh,pixelCenter.x(),pixelCenter.y());
				float z_inbuffer=readZBuffer(x_int,y_int);
				if(interpolatedV.m_pos.z()<=z_inbuffer)//use <= instead of <
				{
					Cfrag frag;
					frag.m_color.init(1,1,1,1);
					drawPixel_OAtLD(hdc,x_int,y_int,frag.m_color);

				}

				x+=DxDivDy;
		
			}

		}

	}else{//x-major line
		//scan col by col
		Cvert vLeft,vRight;
		if(v0.m_pos.x()>v1.m_pos.x()){
			vRight=v0;
			vLeft=v1;
		}else{
			vRight=v1;
			vLeft=v0;
		}//got vLeft,vRight
		int xRight_int,xLeft_int;
		{
			float xRight=vRight.m_pos.x();
			float xLeft=vLeft.m_pos.x();
			xRight_int=floor(xRight+0.5);
			xLeft_int=floor(xLeft+0.5);
		}	
		const double DyDivDx=((double)vRight.m_pos.y()-(double)vLeft.m_pos.y())/((double)vRight.m_pos.x()-(double)vLeft.m_pos.x());
		double x=xLeft_int+0.5;
		double y=(x-vLeft.m_pos.x())*(vRight.m_pos.y()-vLeft.m_pos.y())/(vRight.m_pos.x()-vLeft.m_pos.x())+vLeft.m_pos.y();
		for(int x_int=xLeft_int;x_int<=xRight_int;x_int++){
			int y_int;
			{
				y_int=floor(y+0.5);
			}
			const Cc3dVector2&pixelCenter=Cc3dVector2(x_int+0.5,y_int+0.5);//note: pixel(x_int,y_int)'s center is (x_int+0.5,y_int+0.5)
			Cvert interpolatedV=interpolateInLine_inViewportSpace(vLeft,vRight,pixelCenter.x(),pixelCenter.y());
			float z_inbuffer=readZBuffer(x_int,y_int);
			if(interpolatedV.m_pos.z()<=z_inbuffer)//use <= instead of <
			{
				Cfrag frag;
				frag.m_color.init(1,1,1,1);
				drawPixel_OAtLD(hdc,x_int,y_int,frag.m_color);

			}
			y+=DyDivDx;
		}
		
	}

}




void Cgdi3dScene::fillPanBottomTriFace_depthOnly(HDC hdc,const Cvert&vTop,const Cvert&vBottomLeft,const Cvert&vBottomRight,double polygonOffset){
	//i use below Rasterization Rules:
	//	 xLeft_int=ceil(xLeft-0.5);
	//	 xRight_int=ceil(xRight-1.5);
	//	 yTop_int=ceil(yTop-1.5);
	//	 yBottom_int=ceil(yBottom-0.5);

	float yTop=vTop.m_pos.y();
	float yBottom=vBottomLeft.m_pos.y();
	int yTop_int,yBottom_int;
	{
		
		yTop_int=ceil(yTop-1.5);
		yBottom_int=ceil(yBottom-0.5);

	}
	
	const double leftDxDivDy=((double)vTop.m_pos.x()-(double)vBottomLeft.m_pos.x())/((double)vTop.m_pos.y()-(double)vBottomLeft.m_pos.y());
	const double rightDxDivDy=((double)vTop.m_pos.x()-(double)vBottomRight.m_pos.x())/((double)vTop.m_pos.y()-(double)vBottomRight.m_pos.y());
	double xLeft=lineIntersectWithHorizontalLine(vTop.m_pos,vBottomLeft.m_pos,yBottom_int+0.5);
	double xRight=lineIntersectWithHorizontalLine(vTop.m_pos,vBottomRight.m_pos,yBottom_int+0.5);
	
	for(int y_int=yBottom_int;y_int<=yTop_int;y_int++){
		int xLeft_int,xRight_int;
		{
			xLeft_int=ceil(xLeft-0.5);
			xRight_int=ceil(xRight-1.5);
		}

		for(int x_int=xLeft_int;x_int<=xRight_int;x_int++){
			const Cc3dVector2&pixelCenter=Cc3dVector2(x_int+0.5,y_int+0.5);//note: pixel(x_int,y_int)'s center is (x_int+0.5,y_int+0.5)
			CearlyZOutput earlyZOutput=interpolateInTri_inViewportSpace_earlyZ(vTop,vBottomLeft,vBottomRight,pixelCenter.x(),pixelCenter.y());
			float z_inbuffer=readZBuffer(x_int,y_int);
			if(earlyZOutput.m_z+polygonOffset<z_inbuffer){
				writeZBuffer(x_int,y_int,earlyZOutput.m_z+polygonOffset);



			}

		}


		xLeft+=leftDxDivDy;
		xRight+=rightDxDivDy;

		

	}



}

void Cgdi3dScene::fillPanTopTriFace_depthOnly(HDC hdc,const Cvert&vTopLeft,const Cvert&vBottom,const Cvert&vTopRight,double polygonOffset){
	//i use below Rasterization Rules:
	//	 xLeft_int=ceil(xLeft-0.5);
	//	 xRight_int=ceil(xRight-1.5);
	//	 yTop_int=ceil(yTop-1.5);
	//	 yBottom_int=ceil(yBottom-0.5);

	float yTop=vTopLeft.m_pos.y();
	float yBottom=vBottom.m_pos.y();
    int yTop_int,yBottom_int;
	{
	
		yTop_int=ceil(yTop-1.5);
		yBottom_int=floor(yBottom-0.5);

	}
	const double leftDxDivDy=((double)vTopLeft.m_pos.x()-(double)vBottom.m_pos.x())/((double)vTopLeft.m_pos.y()-(double)vBottom.m_pos.y());
	const double rightDxDivDy=((double)vTopRight.m_pos.x()-(double)vBottom.m_pos.x())/((double)vTopRight.m_pos.y()-(double)vBottom.m_pos.y());
	double xLeft=lineIntersectWithHorizontalLine(vBottom.m_pos,vTopLeft.m_pos,yBottom_int+0.5);
	double xRight=lineIntersectWithHorizontalLine(vBottom.m_pos,vTopRight.m_pos,yBottom_int+0.5);


	for(int y_int=yBottom_int;y_int<=yTop_int;y_int++){
		int xLeft_int,xRight_int;
		{
			xLeft_int=ceil(xLeft-0.5);
			xRight_int=ceil(xRight-1.5);


		}

		for(int x_int=xLeft_int;x_int<=xRight_int;x_int++){
			const Cc3dVector2&pixelCenter=Cc3dVector2(x_int+0.5,y_int+0.5);//note: pixel(x_int,y_int)'s center is (x_int+0.5,y_int+0.5)
			CearlyZOutput earlyZOutput=interpolateInTri_inViewportSpace_earlyZ(vBottom,vTopLeft,vTopRight,pixelCenter.x(),pixelCenter.y());
			float z_inbuffer=readZBuffer(x_int,y_int);
			if(earlyZOutput.m_z+polygonOffset<z_inbuffer){
		
				writeZBuffer(x_int,y_int,earlyZOutput.m_z+polygonOffset);

			}
		}




		xLeft+=leftDxDivDy;
		xRight+=rightDxDivDy;


	}

}

void Cgdi3dScene::fillTriFace_depthOnly(HDC hdc,const Cvert&v0,const Cvert&v1,const Cvert&v2,double polygonOffset){
	//cull back face
	if(m_isCullBackFace){
		if(v0.m_isCullBack){
			if(thisViewportSpaceTriIsFacingUs(v0.m_pos,v1.m_pos,v2.m_pos)==false){
				return;
			}
		}
		
	}

	vector<Cvert> vlist;
	vlist.push_back(v0);
	vlist.push_back(v1);
	vlist.push_back(v2);
	vector<int> downOrderIndexList=getThreeValuesDownOrderIndexList(v0.m_pos.y(),v1.m_pos.y(),v2.m_pos.y());
	const Cvert&vHigh=vlist[downOrderIndexList[0]];
	const Cvert&vMid=vlist[downOrderIndexList[1]];
	const Cvert&vLow=vlist[downOrderIndexList[2]];
	const bool triIsPanTop=(vHigh.m_pos.y()==vMid.m_pos.y());
	const bool triIsPanBottom=(vMid.m_pos.y()==vLow.m_pos.y());
	if(triIsPanTop&&triIsPanBottom){//degenerated tri, actually a horizontal line
		//ignore it
	
	}else if(!triIsPanTop&&triIsPanBottom){//only pan bottom
		if(vLow.m_pos.x()==vMid.m_pos.x()){
			//degenerated tri, actually a line
			//ignore it
		}else if(vLow.m_pos.x()<vMid.m_pos.x()){
			//panBottomTri(vHigh,vLow,vMid)
			fillPanBottomTriFace_depthOnly(hdc,vHigh,vLow,vMid,polygonOffset);
		}else{
			//panBottomTri(vHigh,vMid,vLow)
			fillPanBottomTriFace_depthOnly(hdc,vHigh,vMid,vLow,polygonOffset);
		}


	}else if(triIsPanTop&&!triIsPanBottom){//only pan top
		if(vHigh.m_pos.x()==vMid.m_pos.x()){
			//degenerated tri, actually a line
			//ignore it
		}else if(vHigh.m_pos.x()<vMid.m_pos.x()){
			//panTopTri(vHigh,vLow,vMid)
			fillPanTopTriFace_depthOnly(hdc,vHigh,vLow,vMid,polygonOffset);
		}else{
			//panTopTri(vMid,vLow,vHigh)
			fillPanTopTriFace_depthOnly(hdc,vMid,vLow,vHigh,polygonOffset);
		}

	}else if(!triIsPanTop&&!triIsPanBottom){//not pan bottom and not pan top
		//a horizontal line across vMid intersect with line=(vHigh,vLow) in point v
		Cvert v=linePHighPLowIntersectWithHorizontalLineAcrossPMid(vHigh,vLow,vMid);
		v=interpolateInLine_inViewportSpace(vHigh,vLow,v.m_pos.x(),v.m_pos.y());
			
		if(v.m_pos.x()==vMid.m_pos.x()){
			//triangle is actually a nonHorizon line
			//ignore this degenerated tri
		}else if(v.m_pos.x()<vMid.m_pos.x()){
			//panBottomTri(vHigh,v,vMid) and panTopTri(v,vLow,vMid)
			fillPanBottomTriFace_depthOnly(hdc,vHigh,v,vMid,polygonOffset);
			fillPanTopTriFace_depthOnly(hdc,v,vLow,vMid,polygonOffset);
		}else{
			//panBottomTri(vHigh,vMid,v) and panTopTri(vMid,vLow,v)
			fillPanBottomTriFace_depthOnly(hdc,vHigh,vMid,v,polygonOffset);
			fillPanTopTriFace_depthOnly(hdc,vMid,vLow,v,polygonOffset);
	
		}
	}else{//impossible to reach this branch
		assert(false);
	}
	





}