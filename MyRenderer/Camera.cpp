#include "StdAfx.h"
#include "Vector.h"
#include "Matrix.h"
#include "Camera.h"
#include <math.h>

namespace renderer
{
Camera::Camera(void) //initialise fields to zero
{
	_pitch = 0; 
	_yaw = 0;
	_roll = 0;
	_position = Vector();
	_view = Matrix();
	_projection = Matrix();
	_screen = Matrix();
	_temp = Matrix();
}
Camera::Camera(float pitch, float yaw, float roll, Vector position)
{													//assign given values to fields
	_pitch = pitch;
	_yaw = yaw;
	_roll = roll;
	_position = position;

}
Camera::Camera(Camera& cam) 
{													//copy values into fields
	_pitch = cam._pitch;
	_yaw = cam._yaw;
	_roll = cam._roll;
	_position = cam._position;
	_view = cam._view;
	_projection = cam._projection;
	_screen = cam._screen;
	_temp = cam._temp;

}
Camera::~Camera(void)
{
	//Do nothing
}
void Camera::BuildCameraMatrix(float pitch, float yaw, float roll, Vector Position)
{
	_view = Matrix(1.0f,0.0f,0.0f,Position._x,		//Fixed camera
				 0.0f,1.0f,0.0f,Position._y,
				 0.0f,0.0f,1.0f,Position._z,
				 0.0f,0.0f,0.0f,Position._w);

	//Matrix temp(1.0f,0.0f,0.0f,0.0f,				//Free camera
	//			 0.0f,1.0f,0.0f,0.0f,
	//			 0.0f,0.0f,1.0f,0.0f,
	//			 0.0f,0.0f,0.0f,1.0f);
	//Matrix rot(0.0f,0.0f,0.0f,0.0f,
	//			 0.0f,0.0f,0.0f,0.0f,
	//			 0.0f,0.0f,0.0f,0.0f,
	//			 0.0f,0.0f,0.0f,0.0f);
	//Matrix trans(1.0f,0.0f,0.0f,-Position._x,
	//			 0.0f,1.0f,0.0f,-Position._y,
	//			 0.0f,0.0f,1.0f,-Position._z,
	//			 0.0f,0.0f,0.0f,1.0f);
	//Matrix::Rotate(rot, -pitch, -yaw, -roll);
	//Matrix::Multiply(rot, trans,_view);
}
void Camera::BuildProjectionMatrix(float fov, float w, float width, float height)
{
	float ar = (width / height);					//Calculate Aspect Ratio
	float d = ((w/2)/(tan(fov/2)));					//Calculate d
	_projection = Matrix(d/ar,0.0f,0.0f,0.0f,
						 0.0f,d,0.0f,0.0f,
						 0.0f,0.0f,d,0.0f,
						 0.0f,0.0f,1.0f,0.0f);
}
void Camera::BuildScreenMatrix(float width, float height)
{
	float a = width/2;
	float b = height/2;								
	
	_screen = Matrix(a,0.0f,0.0f,a,
				  0.0f,-b,0.0f,b,
				  0.0f,0.0f,1.0f,0.0f,
				  0.0f,0.0f,0.0f,1.0f);
	
}
}