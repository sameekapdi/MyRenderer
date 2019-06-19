// MyRenderer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <stdlib.h>
#include <crtdbg.h>
#include <commctrl.h>
#include "MyRenderer.h"
#include "MyApp.h"
#include "MD2Loader.h"
#include <gdiplus.h>

using namespace renderer;
using namespace Gdiplus;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND hWnd;										// window handler
MyApp app;										// Renderer

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;
	// Initialize GDI+.
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MYRENDERER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYRENDERER));

	//Initialise Renderer
	app.Init(hWnd);

	// Main message loop:
	while (true)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			// test if this is a quit
			if (msg.message == WM_QUIT)
				break;
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		app.Update();
	}
	app.Shutdown();
	
	GdiplusShutdown(gdiplusToken);
	return (int) msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYRENDERER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MYRENDERER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(
	      szWindowClass,				// window class name
	      szTitle,						// window caption
	      WS_OVERLAPPEDWINDOW,			// window style
		  0,							// initial x position
	      0,							// initial y position
	      640,							// initial x size
	      480,							// initial y size
	      NULL,							// parent window handle
	      NULL,							// window menu handle
	      hInstance,					// program instance handle
	      NULL);						// creation parameters
  


   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
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
		//Keyboard input
	case WM_KEYDOWN:
			if (wParam == '1')
			{
				//Render verts
				app._object->SetDrawType(_VERTS);
			}
			if (wParam == '2')
			{
				//Render wireframe
				app._object->SetDrawType(_WIREFRAME);
			}
			if (wParam == '3')
			{
				//Render Gdiplus flat shading
				app._object->SetDrawType(_FLAT_GDIPLUS);
			}
			if (wParam == '4')
			{
				//Render flat shading
				app._object->SetDrawType(_FLAT);
			}
			if (wParam == '5')
			{
				//Render Gouraud shading
				app._object->SetDrawType(_GOURAUD);
			}
			if (wParam == 'Q')
			{
				//Load a cube
				MD2Loader::LoadModel("cube.md2", *app._object);
				app._camera->_position._z = 75.0f;
			}
			if (wParam == 'W')
			{
				//Load a helicopter
				MD2Loader::LoadModel("helicopter.md2", *app._object);
				app._camera->_position._z = 250.0f;
			}
			if (wParam == 'E')
			{
				//Load a ball
				MD2Loader::LoadModel("ball.md2", *app._object);
				app._camera->_position._z = 150.0f;
			}

			if (wParam == 'R')
			{
				//Load a plant
				MD2Loader::LoadModel("Plant.md2", *app._object);
				app._camera->_position._z = 75.0f;
			}
			if (wParam == 'T')
			{//Load a car
				MD2Loader::LoadModel("car.md2", *app._object);
				app._camera->_position._z = 250.0f;
			}
			if (wParam == 'Y')
			{//Load a cuboid
				MD2Loader::LoadModel("pole.md2", *app._object);
				app._camera->_position._z = 150.0f;
			}
			if (wParam == 'A')
			{
				//Make lights red
				app._lights->_Ld = Gdiplus::Color(255,255,000,000);
			}
			if (wParam == 'S')
			{
				//Increase red intensity
				float r = app._lights->_Ld.GetRed();
				float g = app._lights->_Ld.GetGreen();
				float b = app._lights->_Ld.GetBlue();
				if (r <255)
				{r+=1;}
				app._lights->_Ld = Gdiplus::Color(255,r,g,b);
			}
			if (wParam == 'D')
			{
				//Decrease red intensity
				float r = app._lights->_Ld.GetRed();
				float g = app._lights->_Ld.GetGreen();
				float b = app._lights->_Ld.GetBlue();
				if (r >0)
				{r-=1;}
				app._lights->_Ld = Gdiplus::Color(255,r,g,b);
			}
			if (wParam == 'F')
			{
				//make lights green
				app._lights->_Ld = Gdiplus::Color(255,000,255,000);
			}
			if (wParam == 'G')
			{
				//increase green intensity
				float r = app._lights->_Ld.GetRed();
				float g = app._lights->_Ld.GetGreen();
				float b = app._lights->_Ld.GetBlue();
				if (g <255)
				{g+=1;}
				app._lights->_Ld = Gdiplus::Color(255,r,g,b);
			}
			if (wParam == 'H')
			{
				//decrease green intensity
				float r = app._lights->_Ld.GetRed();
				float g = app._lights->_Ld.GetGreen();
				float b = app._lights->_Ld.GetBlue();
				if (g >0)
				{g-=1;}
				app._lights->_Ld = Gdiplus::Color(255,r,g,b);
			}
			if (wParam == 'J')
			{
				//make lights blue
				app._lights->_Ld = Gdiplus::Color(255,000,000,255);
			}
			if (wParam == 'K')
			{
				//increase light intensity
				float r = app._lights->_Ld.GetRed();
				float g = app._lights->_Ld.GetGreen();
				float b = app._lights->_Ld.GetBlue();
				if (b <255)
				{b+=1;}
				app._lights->_Ld = Gdiplus::Color(255,r,g,b);
			}
			if (wParam == 'L')
			{
				//decrease light intensity
				float r = app._lights->_Ld.GetRed();
				float g = app._lights->_Ld.GetGreen();
				float b = app._lights->_Ld.GetBlue();
				if (b >0)
				{b-=1;}
				app._lights->_Ld = Gdiplus::Color(255,r,g,b);
			}
			if (wParam == 'Z')
			{
				//increase red ambience
				if (app._lights->_ka_red < 1.0f)
				{app._lights->_ka_red+=0.1f;}
			}
			if (wParam == 'X')
			{
				//increase green ambience
				if (app._lights->_ka_green< 1.0f)
				{app._lights->_ka_green+=0.1f;}
			}
			if (wParam == 'C')
			{
				//increase blue ambience
				if (app._lights->_ka_blue < 1.0f)
				{app._lights->_ka_blue+=0.1f;}
			}
			if (wParam == 'B')
			{
				//decrease red ambience
				if (app._lights->_ka_red > 0.0f)
				{app._lights->_ka_red-=0.1f;}
			}
			if (wParam == 'N')
			{
				//decrease green ambience
				if (app._lights->_ka_green > 0.0f)
				{app._lights->_ka_green-=0.1f;}
			}
			if (wParam == 'M')
			{
				//decrease blue ambience
				if (app._lights->_ka_blue > 0.0f)
				{app._lights->_ka_blue-=0.1f;}
			}
			if (wParam == '7')
			{
				//increase red reflectance
				if (app._lights->_kd_red < 1.0f)
				{app._lights->_kd_red+=0.1f;}
			}
			if (wParam == '8')
			{
				//increase green reflectance
				if (app._lights->_kd_green < 1.0f)
				{app._lights->_kd_green+=0.1f;}
			}
			if (wParam == '9')
			{
				//increase blue reflectance
				if (app._lights->_kd_blue < 1.0f)
				{app._lights->_kd_blue+=0.1f;}
			}
			if (wParam == 'I')
			{
				//decrease red reflectance
				if (app._lights->_kd_red > 0.0f)
				{app._lights->_kd_red-=0.1f;}
			}
			if (wParam == 'O')
			{
				//decrease green reflectance
				if (app._lights->_kd_green > 0.0f)
				{app._lights->_kd_green-=0.1f;}
			}
			if (wParam == 'P')
			{
				//decrease blue reflectance
				if (app._lights->_kd_blue > 0.0f)
				{app._lights->_kd_blue-=0.1f;}
			}
			if (wParam == '0')
			{
				//reset light
				*app._lights = Lights(Gdiplus::Color(255,000,000,000),Gdiplus::Color(255,255,255,255),0.1f,0.1f,0.1f,Gdiplus::Color(255,000,255,000),1.0f,1.0f,1.0f,Vector(0.0f,0.0f,700.0f,1.0f));
			}
			if (wParam == VK_UP)
			{
				// move light up
				app._lights->_source._y+=100;
			}
			if (wParam == VK_DOWN)
			{
				//move light down
				app._lights->_source._y-=100;
			}
			if (wParam == VK_LEFT)
			{
				// move light left
				app._lights->_source._x+=100;
			}
			if (wParam == VK_RIGHT)
			{
				// move light right
				app._lights->_source._x-=100;
			}
			break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
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