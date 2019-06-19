#include "StdAfx.h"
#include "MyApp.h"
#include "MD2Loader.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace renderer;

MyApp::MyApp(void) //default constructor
{}


MyApp::~MyApp(void) //destructor
{
	// will call explicitly in shutdowns
}

void MyApp::Init(HWND hWnd)
{
	_graphics = new Gdiplus::Graphics(hWnd);
	// construct the camera
	_camera = new Camera((0*((M_PI)/180)),(0*((M_PI)/180)),(0*((M_PI)/180)),Vector(0.0f,0.0f,150.0f,1.0f));
	// construct the rasterizer
	_rasterizer = new Rasterizer(640,480);
	// construct an object#
	_object = new Object();
	// load a model into the object
	MD2Loader::LoadModel("ball.md2", *_object);
	//construct a light
	_lights = new Lights(Gdiplus::Color(255,000,000,000),Gdiplus::Color(255,255,255,255),0.1f,0.1f,0.1f,Gdiplus::Color(255,000,255,000),1.0f,1.0f,1.0f,Vector(0.0f,0.0f,700.0f,1.0f));
}

void MyApp::Update(void)
{
	Render();
}
float rotx =0; //create totation values
float roty =1;
float rotz =2;
void MyApp::Render(void)
{
	Matrix temp;
	rotx+=0.01f; roty+=0.01f; rotz+=0.01f;  //increase rotation values
	if (rotx>2)								//if rotation values get too large, invert them
	{rotx = -2;}
	if (roty>2)
	{roty = -2;}
	if (rotz>2)
	{rotz = -2;}

	Matrix::Rotate(temp,(rotx*((M_PI)/180)),(roty*((M_PI)/180)),(rotz*((M_PI)/180))); //rotate the object
	_object->Transform(temp,_LOCAL_TO_TRANS);			//transform into world space

	_object->CalculatePolygonNormals();					//find the polygon normals
	
	_object->CalculateBackfaces(_camera->_position);	//calculate the back faces for culling
	_object->Sort();									//sort polygons

	//build view matrix
	_camera->BuildCameraMatrix(_camera->_pitch, _camera->_yaw, _camera->_roll,_camera->_position); 
	_object->Transform(_camera->_view, _TRANS );		//transform into camera space
	_object->CalculateLightingAmbient(*_lights);		//calculate ambient lighting

			switch(_object->_drawType)
		{
			case E_DRAW_TYPE::_GOURAUD:					//if the draw type is gouraud shading
				_object->CalculateVertexNormals();		//calculate vertex normals and lighting
				_object->CalculateLightingDirectionalGouraud(*_lights);
				_object->CalculateLightingPointGouraud(*_lights);
			break;
			case E_DRAW_TYPE::_FLAT:					//if draw type is flat shading
			case E_DRAW_TYPE::_FLAT_GDIPLUS:			//calculate normal lighting
				_object->CalculateLightingDirectionalFlat(*_lights);
				_object->CalculateLightingPointFlat(*_lights);
			break;
			default:
			break;
		}
	
	_camera->BuildProjectionMatrix(M_PI_2,2,640.0f,480.0f);		//build perspective projection matrix
	_object->Transform( _camera->_projection, _TRANS );			//transform into perspective space

	_object->Dehomogenise();									//flatten w to 1

	_camera->BuildScreenMatrix(640.0f,480.0f);					//build screen matrix
	_object->Transform( _camera->_screen, _TRANS );				//transform to screen space

	_rasterizer->Clear();										//refresh rasterizer
	
			switch(_object->_drawType)							//depending on the draw type
		{
		case E_DRAW_TYPE::_VERTS:
			_rasterizer->DrawVerts(*_object);					//draw each vertex
			break;
		case E_DRAW_TYPE::_WIREFRAME:
			_rasterizer->DrawWireframe(*_object);				//draw wireframe
			break;
		case E_DRAW_TYPE::_FLAT_GDIPLUS:
			_rasterizer->DrawSolidGDI(*_object);				//draw flat shading with gdiplus
			break;
		case E_DRAW_TYPE::_FLAT:
			_rasterizer->DrawSolidFlat(*_object);				//draw flat shading
			break;
		case E_DRAW_TYPE::_GOURAUD:
			_rasterizer->DrawSolidGouraud(*_object);			//draw gouraud shading
			break;
		}
	_rasterizer->DrawFPS();										//draw fps

	_graphics->DrawImage(_rasterizer->_bitmap,0,0,640,480);		//copy our image to screen
	

}

void MyApp::Shutdown(void)
{
	// delete members
	delete _camera;
	delete _object;
	delete _rasterizer;
	delete _graphics;
	delete _lights;
}
