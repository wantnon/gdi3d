
#include "mesh.h"
#include "c3dCamera.h"
#include "c3dLightSource.h"
#include "c3dMaterial.h"
#include "texture.h"
#include "funcs.h"
//opengl transform pipeline,ref: http://user.qzone.qq.com/350479720/blog/1310060277
enum eSamepleScene{
	
	eSampleScene_cube=0,
	eSampleScene_ball,
	eSampleScene_girl,
	eSampleScene_teaport,

	eSampleSceneCount,
};
enum eDrawMode{
	eDrawSolid=0,
	eDrawEdge,

	eDrawModeCount,
};


//----shader program
//this two functions imitate vertexShaderProgram and fragmentShaderProgram in Opengl 2.0, you can change their implementation to test other lighting strategy
Cvert vertexShaderProgram(const Cc3dMatrix4&modelMat,const Cc3dCamera*camera,const Cc3dLightSource*lightSource,const Cc3dMaterial*material,const Cvert&v);
Cfrag fragShaderProgram(const Cvert&interpolatedV,const Ctexture*texture);

class Cgdi3dScene;	
extern Cgdi3dScene*gdi3dScene;

class Cgdi3dScene{
public:
	Cgdi3dScene(){
		m_modelMat=unitMat();
		m_isCullBackFace=true;
		m_lightSource=NULL;
		m_camera=NULL;
		m_mesh=NULL;
		m_transformedMesh=NULL;
		m_zBuffer=NULL; 
		m_drawMode=eDrawSolid;
	    m_polygonOffsetForEdgeMode_factor=1.0;
	    m_polygonOffsetForEdgeMode_units=1.0;
		m_polygonOffsetForAllMode_r=0.0001;
	}
	virtual~Cgdi3dScene(){
		if(m_lightSource)delete m_lightSource;
		if(m_camera)delete m_camera;
		if(m_mesh)delete m_mesh;
		if(m_transformedMesh)delete m_transformedMesh;
		if(m_zBuffer)delete[]m_zBuffer; 
		for(int i=0;i<(int)m_materialList.size();i++){
			delete m_materialList[i];
		}m_materialList.clear();
		for(int i=0;i<(int)m_textureList.size();i++){
			delete m_textureList[i];
		}m_textureList.clear();
	}
	void init(Cc3dVector4 viewport){
		
		//viewport and viewportMat 
		m_viewport=viewport;
		m_viewportMat=calculateViewportMat(m_viewport);
		//create camera
		m_camera=new Cc3dCamera(viewport.z(),viewport.w());
		m_camera->setEyePos(cc3dv4(1,0,2,1));
		m_camera->setCenter(cc3dv4(0,0,0,1));
		m_camera->setUp(cc3dv4(0,1,0,0));
		
		//m_zBuffer
		m_zBuffer=new double[(int)m_viewport.z()*(int)m_viewport.w()];
		clearZBuffer();
		//crate lightSource
		m_lightSource=new Cc3dLightSource();
		m_lightSource->m_pos=cc3dv4(-50*10,100*10,70*10,1);
		m_lightSource->m_ambient=cc3dv4(1,1,1,1);

		//create mesh
		m_mesh=new Cmesh();

		//create transformedMesh
		m_transformedMesh=new Cmesh();

		//make sample scene
	    makeSampleScene_cube();
	 
		

	}
public://got info
	int getTriangleCountBeforeClipping()const {return (int)m_mesh->m_IDtriList.size();}
	int getVertexCountBeforeClipping()const {return (int)m_mesh->m_positionList.size();}
public://make sample scene
	void makeSampleScene_cube();
	void makeSampleScene_ball();
	void makeSampleScene_girl();
	void makeSampleScene_teaport();
public://rasterization high level functions
	void softRasterization(HDC hdc);
	void submitMesh();
	void transformVertexes();
	void doClippingInClippingSpace();
	void rasterizeTriangles_solid(HDC hdc);
	void rasterizeTriangles_edge(HDC hdc);
public://assist funcs
	void resetScene();
	void loadModelMyFormat(const string&folderPath,const string&fileName,Cmesh*mesh);
	void loadModelASE(const string&folderPath,const string&fileName,Cmesh*mesh);
	void storeInitialCameraSetting();
	void storeInitialModelMat();
	void restoreToInitialCameraSettting();
	void restoreToInitialModelMat();
public://rasterization low level functions
	//calculate viewport mat
	Cc3dMatrix4 calculateViewportMat(Cc3dVector4 viewport);	
	//clipping
	void roughTriRejectInClippingSpace();
	bool isTriTotallyLeftOutOfClippingSpace(const Cc3dVector4&p0,const Cc3dVector4&p1,const Cc3dVector4&p2);
	bool isTriTotallyRightOutOfClippingSpace(const Cc3dVector4&p0,const Cc3dVector4&p1,const Cc3dVector4&p2);
	bool isTriTotallyTopOutOfClippingSpace(const Cc3dVector4&p0,const Cc3dVector4&p1,const Cc3dVector4&p2);
	bool isTriTotallyBottomOutOfClippingSpace(const Cc3dVector4&p0,const Cc3dVector4&p1,const Cc3dVector4&p2);
	bool isTriTotallyFarOutOfClippingSpace(const Cc3dVector4&p0,const Cc3dVector4&p1,const Cc3dVector4&p2);
	bool isTriTotallyNearOutOfClippingSpace(const Cc3dVector4&p0,const Cc3dVector4&p1,const Cc3dVector4&p2);
	bool isTriMustBeOutOfClippingSpace(const Cc3dVector4&p0,const Cc3dVector4&p1,const Cc3dVector4&p2);
	void clipAgainstNearPlaneInClippingSpace();
	void clipAgainstFarPlaneInClippingSpace();
	void clipAgainstLeftPlaneInClippingSpace();
	void clipAgainstRightPlaneInClippingSpace();
	void clipAgainstTopPlaneInClippingSpace();
	void clipAgainstBottomPlaneInClippingSpace();
	vector<Ctri> cutTriWithNearPlaneInClippingSpace(const Cvert&v0,const Cvert&v1,const Cvert&v2,float Near);
	vector<Ctri> cutTriWithFarPlaneInClippingSpace(const Cvert&v0,const Cvert&v1,const Cvert&v2,float Far);
	vector<Ctri> cutTriWithLeftPlaneInClippingSpace(const Cvert&v0,const Cvert&v1,const Cvert&v2,float Left);
	vector<Ctri> cutTriWithRightPlaneInClippingSpace(const Cvert&v0,const Cvert&v1,const Cvert&v2,float Right);
	vector<Ctri> cutTriWithTopPlaneInClippingSpace(const Cvert&v0,const Cvert&v1,const Cvert&v2,float Top);
	vector<Ctri> cutTriWithBottomPlaneInClippingSpace(const Cvert&v0,const Cvert&v1,const Cvert&v2,float Bottom);
	//cull face
	bool thisViewportSpaceTriIsFacingUs(const Cc3dVector4&p0,const Cc3dVector4&p1,const Cc3dVector4&p2);
	//interpolate
	Cvert interpolateInLine(const Cvert&v1,const Cvert&v2,float x,float y);
	Cvert interpolateInLine_inClippingSpace(const Cvert&v1,const Cvert&v2,float x,float y);
	CearlyZOutput interpolateInTri_earlyZ(const Cvert&v0,const Cvert&v1,const Cvert&v2,float x,float y);
	Cvert interpolateInTri_otherAttributes(const Cvert&v0,const Cvert&v1,const Cvert&v2,const CearlyZOutput&earlyZOutput);
	//zbuffer
	void clearZBuffer();
	double readZBuffer(int x,int y);
	void writeZBuffer(int x,int y,double zValueToWrite);
	//draw pixel
	void drawPixel_OAtLD(HDC hdc,float x,float y,const Cc3dVector4&color);//suppose origin is at left down corner
	//triangle rasterization (solid mode)
	void fillTriFace_solid(HDC hdc,const Cvert&v0,const Cvert&v1,const Cvert&v2);
	void fillPanBottomTriFace_solid(HDC hdc,const Cvert&vTop,const Cvert&vBottomLeft,const Cvert&vBottomRight);
	void fillPanTopTriFace_solid(HDC hdc,const Cvert&vTopLeft,const Cvert&vBottom,const Cvert&vTopRight);
	//traingle rasterization (edge mode)
	void fillLine(HDC hdc,const Cvert&v0,const Cvert&v1);
	void fillTriFace_edge(HDC hdc,const Cvert&v0,const Cvert&v1,const Cvert&v2);
	void fillTriFace_depthOnly(HDC hdc,const Cvert&v0,const Cvert&v1,const Cvert&v2,double polygonOffset);
	void fillPanBottomTriFace_depthOnly(HDC hdc,const Cvert&vTop,const Cvert&vBottomLeft,const Cvert&vBottomRight,double polygonOffset);
	void fillPanTopTriFace_depthOnly(HDC hdc,const Cvert&vTopLeft,const Cvert&vBottom,const Cvert&vTopRight,double polygonOffset);
public://functions only for debug
	void drawLine_OAtLD(HDC hdc,float x1,float y1,float x2,float y2);//suppose origin is at left down corner
	void drawTriWire(HDC hdc,const Cc3dVector4&p0,const Cc3dVector4&p1,const Cc3dVector4&p2);
public://lightSource
	Cc3dLightSource*m_lightSource;
public://camera
	Cc3dCamera*m_camera;
	Cc3dVector4 m_initialCameraEyePos;
	Cc3dVector4 m_initialCameraUp;
	Cc3dVector4 m_initialCameraCenter;
public://model
	Cmesh* m_mesh;
	Cc3dMatrix4 m_modelMat;
	Cc3dMatrix4 m_initialModelMat;
	vector<Cc3dMaterial*> m_materialList;
	vector<Ctexture*> m_textureList;
public://config
	bool m_isCullBackFace;
	int m_drawMode;
public://pipline
	Cmesh* m_transformedMesh;
	double* m_zBuffer; 
	Cc3dVector4 m_viewport;
	Cc3dMatrix4 m_viewportMat;
	double m_polygonOffsetForEdgeMode_factor;
	double m_polygonOffsetForEdgeMode_units;
	double m_polygonOffsetForAllMode_r;

};
