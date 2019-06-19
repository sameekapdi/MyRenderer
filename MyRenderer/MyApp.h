#pragma once

#include "Rasterizer.h"
#include "Camera.h"
#include "Object.h"
namespace renderer
{
class MyApp
{
public:
	MyApp(void); //constructor
	~MyApp(void); //destructor

public://operations
	void Init(HWND hWnd); 
	void Update(void);  
	void Render(void);  
	void Shutdown(void); 

public://fields
	renderer::Rasterizer* _rasterizer;
	renderer::Camera*	 _camera;
	renderer::Object*	 _object;
	renderer::Lights*	 _lights;
private:
	Gdiplus::Graphics* _graphics;

};
}