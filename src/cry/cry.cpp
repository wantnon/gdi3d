// cry.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include<iostream>

using namespace std;
#include "cry.h"
#include "myDefine.h"
#include "gdi3d.h"
#include <time.h>
#include "myFuncsForWin.h"
#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	//----begin yang chao
	//----create console
	AllocConsole();   // 建立控制台
    SetConsoleTitle(_T("Debug Output"));      // 设置控制台窗口标题
	// 重定向 STDIN
	{
		FILE*fp;
		errno_t eno=freopen_s(&fp,"CONIN$", "r+t", stdin);
		if(eno!=0){//不成功
			MessageBox(NULL,_T("重定向stdin失败!"),_T("error"),MB_OK|MB_ICONEXCLAMATION);
			exit(0);
		}
	}
	// 重定向STDOUT
	{
		FILE*fp;
		errno_t eno=freopen_s(&fp,"CONOUT$", "w+t", stdout);
		if(eno!=0){//不成功
			MessageBox(NULL,_T("重定向stdout失败!"),_T("error"),MB_OK|MB_ICONEXCLAMATION);
			exit(0);
		}
	
	}
	//----end yang chao

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CRY, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CRY));


	
	

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	//----begin yang chao
	//destroy console
	FreeConsole();//销毁控制台
	//----end yang chao

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CRY));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;//(HBRUSH)(COLOR_WINDOW+1);//change this value to NULL, ref:《AI Techniques for Game Programming》
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_CRY);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   //forbid user resize the window, ref:http://www.cnblogs.com/ifaithu/archive/2012/03/26/2417486.html
    hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW^WS_THICKFRAME,
      CW_USEDEFAULT, 0, g_winWidth, g_winHeight, NULL, NULL, hInstance, NULL);//hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
   if (!hWnd)
   {
      return FALSE;
   }

   //----begin yang chao
   srand((int)time(0));
   //use clientRect to set viewport
   RECT rect;
   GetClientRect(hWnd,&rect);
   cout<<"----clientRect:"<<endl;
   cout<<"rect.bottom:"<<rect.bottom<<endl;
   cout<<"rect.left:"<<rect.left<<endl;
   cout<<"rect.right:"<<rect.right<<endl;
   cout<<"rect.top:"<<rect.top<<endl;
   Cc3dVector4 viewport(rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top);
   //init gdi3d
   cout<<"----init gdi3d"<<endl;
   gdi3dScene=new Cgdi3dScene();
   gdi3dScene->init(viewport);

 
   //----end yang chao


   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	
	//implementation of double buffer, ref:《AI Techniques for Game Programming》
	//----begin yang chao
	static int cxClient, cyClient;
	static HDC hdcBackBuffer;
	static HBITMAP hBitmap;
	static HBITMAP hOldBitmap;
	//----end yang chao
	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_CREATE:
		{
			//获取客户区尺寸
			RECT rect;
			GetClientRect (hWnd,&rect);
			cxClient=rect.right;
			cyClient=rect.bottom;
			//创建后备缓冲区
			//先创建一个内存DC
			hdcBackBuffer=CreateCompatibleDC(NULL);
			//获得前置缓冲区的DC
			HDC hdc=GetDC(hWnd);
			hBitmap=CreateCompatibleBitmap(hdc,cxClient,cyClient);
			//将bitmap装入内存DC
			hOldBitmap=(HBITMAP)SelectObject(hdcBackBuffer,hBitmap);
			//释放DC
			ReleaseDC(hWnd,hdc);
		}
		break;

	case WM_PAINT:
		{
			hdc = BeginPaint(hWnd, &ps);
			// TODO: 在此添加任意绘图代码...
			//用白色填充backbuffer
			BitBlt(hdcBackBuffer,
				0,
				0,
				cxClient,
				cyClient,
				NULL,
				NULL,
				NULL,
				BLACKNESS);//WHITENESS);
			//--------------------------------------------------------
			//在这里执行所有的画图操作
			clock_t start=clock();//paint start clock
			{
				//--draw scene
				gdi3dScene->softRasterization(hdcBackBuffer);
			
			}
			clock_t stop=clock();//paint end clock
			//calculate fps
			double dur=(double)(stop-start)/(double)CLOCKS_PER_SEC;
			int fps=(int)min(1000,1.0/dur);
			//--draw text
			SetTextColor(hdcBackBuffer,RGB(255,255,255));
			SetBkMode(hdcBackBuffer,TRANSPARENT);
			string numstr_triangleCount=numberToStr(gdi3dScene->getTriangleCountBeforeClipping());
			string numstr_vertexCount=numberToStr(gdi3dScene->getVertexCountBeforeClipping());
			TextOut(hdcBackBuffer,0,5,string("nVert:")+numstr_vertexCount);
			TextOut(hdcBackBuffer,0,5+20,string("nTri:")+numstr_triangleCount);
			TextOut(hdcBackBuffer,0,5+20*2,string("fps:")+numberToStr(fps));
			TextOut(hdcBackBuffer,0,5+20*3,"A,S,D,W: move camera");
			TextOut(hdcBackBuffer,0,5+20*4,"Q,E: swing camera");
			TextOut(hdcBackBuffer,0,5+20*5,"mouse down&move: camera dir");
			TextOut(hdcBackBuffer,0,5+20*6,"direction keys: rotate scene");
			TextOut(hdcBackBuffer,0,5+20*7,"F1: solid / edge");
			TextOut(hdcBackBuffer,0,5+20*8,"F2: reset");
			TextOut(hdcBackBuffer,0,5+20*9,"N: next scene");
			//--------------------------------------------------------
			//现在把back buffer内容用位块传送方法传到front buffer
			BitBlt(ps.hdc,
				0,
				0,
				cxClient,
				cyClient,
				hdcBackBuffer,
				0,
				0,
				SRCCOPY);
			EndPaint(hWnd, &ps);
		
		}
		break;
	case WM_DESTROY:
		//----begin yang chao
		SelectObject(hdcBackBuffer,hOldBitmap);
		DeleteDC(hdcBackBuffer);
		DeleteObject(hBitmap);
		//----end yang chao
		//----begin yang chao
		delete gdi3dScene;
		//----end yang chao

		PostQuitMessage(0);
		break;
	case WM_MOUSEMOVE:
		{
			int xPos = LOWORD(lParam); 
			int yPos = HIWORD(lParam);
			g_mosPosFoe=g_mosPos;
			g_mosPos=Cc3dVector2(xPos,yPos);
			
			if(g_mosIsLDown){
				float dx=g_mosPos.x()-g_mosPosFoe.x();
				float dy=g_mosPos.y()-g_mosPosFoe.y();
				float dA_horizontalRot=-dx*0.05;
				float dA_verticalRot=-dy*0.05;
				gdi3dScene->m_camera->rotate(cc3dv4(0,1,0,0),cos(dA_horizontalRot*M_PI/180),sin(dA_horizontalRot*M_PI/180));
				gdi3dScene->m_camera->rotate(cc3dv4(1,0,0,0),cos(dA_verticalRot*M_PI/180),sin(dA_verticalRot*M_PI/180));
				InvalidateRect(hWnd,NULL,false);
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			int xPos = LOWORD(lParam); 
			int yPos = HIWORD(lParam);
		

			
			g_mosPosFoe=g_mosPos;
			g_mosPos=Cc3dVector2(xPos,yPos);
	
			g_mosIsLDown=true;
			////cout<<"mos:"<<xPos<<" "<<yPos<<endl;
		}
		break;
	case WM_LBUTTONUP:
		{
			int xPos = LOWORD(lParam); 
			int yPos = HIWORD(lParam);
		

			
			g_mosPosFoe=g_mosPos;
			g_mosPos=Cc3dVector2(xPos,yPos);

			g_mosIsLDown=false;
		}
		break;
	case WM_KEYDOWN:
		{
			//vk table ref: http://www.cnblogs.com/mr-wid/archive/2012/11/20/2779781.html
			switch(wParam)
			{
			case 65://a
				{
					Cc3dVector4 movVec=cc3dv4(-1,0,0,0)*0.1;
					Cc3dVector4 oldEyePos=gdi3dScene->m_camera->getEyePos();
					gdi3dScene->m_camera->setEyePos(oldEyePos+movVec);
					Cc3dVector4 oldCenter=gdi3dScene->m_camera->getCenter();
					gdi3dScene->m_camera->setCenter(oldCenter+movVec);
					InvalidateRect(hWnd,NULL,false);
				}
				break;
			case 83://s
				{
					Cc3dVector4 movVec=cc3dv4(0,0,1,0)*0.1;
					Cc3dVector4 oldEyePos=gdi3dScene->m_camera->getEyePos();
					gdi3dScene->m_camera->setEyePos(oldEyePos+movVec);
					Cc3dVector4 oldCenter=gdi3dScene->m_camera->getCenter();
					gdi3dScene->m_camera->setCenter(oldCenter+movVec);
					InvalidateRect(hWnd,NULL,false);
				}
				break;
			case 87://w
				{
					Cc3dVector4 movVec=cc3dv4(0,0,-1,0)*0.1;
					Cc3dVector4 oldEyePos=gdi3dScene->m_camera->getEyePos();
					gdi3dScene->m_camera->setEyePos(oldEyePos+movVec);
					Cc3dVector4 oldCenter=gdi3dScene->m_camera->getCenter();
					gdi3dScene->m_camera->setCenter(oldCenter+movVec);
					InvalidateRect(hWnd,NULL,false);
				}
				break;
			case 68://d
				{
					Cc3dVector4 movVec=cc3dv4(1,0,0,0)*0.1;
					Cc3dVector4 oldEyePos=gdi3dScene->m_camera->getEyePos();
					gdi3dScene->m_camera->setEyePos(oldEyePos+movVec);
					Cc3dVector4 oldCenter=gdi3dScene->m_camera->getCenter();
					gdi3dScene->m_camera->setCenter(oldCenter+movVec);
					InvalidateRect(hWnd,NULL,false);
				}

				break;
			case 81://q
				{
					float dA=1;
					gdi3dScene->m_camera->rotate(cc3dv4(0,0,1,0),cos(dA*M_PI/180),sin(dA*M_PI/180));
					InvalidateRect(hWnd,NULL,false);

				}
				break;
			case 69://e
				{
					float dA=-1;
					gdi3dScene->m_camera->rotate(cc3dv4(0,0,1,0),cos(dA*M_PI/180),sin(dA*M_PI/180));
					InvalidateRect(hWnd,NULL,false);
				}
				break;
			case 78://n
				{

					g_curSampleScene=(g_curSampleScene+1)%eSampleSceneCount;
					switch(g_curSampleScene){
					
					case eSampleScene_cube:
						gdi3dScene->makeSampleScene_cube();
						break;
					case eSampleScene_ball:
						gdi3dScene->makeSampleScene_ball();
						break;
					case eSampleScene_girl:
						gdi3dScene->makeSampleScene_girl();
						break;
					case eSampleScene_teaport:
						gdi3dScene->makeSampleScene_teaport();
						break;
				
					}
					InvalidateRect(hWnd,NULL,false);
				}
				break;
			case 112://F1
				{
	
					
					gdi3dScene->m_drawMode=(gdi3dScene->m_drawMode+1)%eDrawModeCount;
					InvalidateRect(hWnd,NULL,false);
						
				}
				break;
			case 113://F2
				{
					gdi3dScene->restoreToInitialCameraSettting();
					gdi3dScene->restoreToInitialModelMat();
					InvalidateRect(hWnd,NULL,false);

				}
				break;
			case VK_LEFT:
				{
					float dA=-5;
					gdi3dScene->m_modelMat=calculateRotationMatrix(cc3dv4(0,1,0,0),cos(dA*M_PI/180),sin(dA*M_PI/180))*gdi3dScene->m_modelMat;
					InvalidateRect(hWnd,NULL,false);
				}
				break;
			case VK_RIGHT:
				{
					float dA=5;
					gdi3dScene->m_modelMat=calculateRotationMatrix(cc3dv4(0,1,0,0),cos(dA*M_PI/180),sin(dA*M_PI/180))*gdi3dScene->m_modelMat;
					InvalidateRect(hWnd,NULL,false);
				}
				break;
			case VK_UP:
				{
					float dA=5;
					gdi3dScene->m_modelMat=calculateRotationMatrix(cc3dv4(-1,0,0,0),cos(dA*M_PI/180),sin(dA*M_PI/180))*gdi3dScene->m_modelMat;
					InvalidateRect(hWnd,NULL,false);
				}
				break;
			case VK_DOWN:
				{
					float dA=5;
					gdi3dScene->m_modelMat=calculateRotationMatrix(cc3dv4(1,0,0,0),cos(dA*M_PI/180),sin(dA*M_PI/180))*gdi3dScene->m_modelMat;
					InvalidateRect(hWnd,NULL,false);
				}
				break;
			}
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
