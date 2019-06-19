#pragma once
#include "Vector.h"
#include "Matrix.h"
namespace renderer
{
	//forward declarations
class Vector;
class Matrix;

class Camera
{
public: // constructors
	Camera(void); //default constructor
	Camera(float pitch, float yaw, float roll, Vector position); //parameterised constructor
	Camera(Camera&); //copy constructor
	~Camera(void); //destructor
public: // operations
	void BuildCameraMatrix(float pitch, float yaw, float roll, Vector position);
	void BuildProjectionMatrix(float fov, float w, float width, float height);
	void BuildScreenMatrix(float width, float height);
	void OutputToConsole(); //output

public: //fields
	Matrix _view;
	Matrix _projection;
	Matrix _screen;
	Matrix _temp;

	float _pitch;
	float _yaw;
	float _roll;
	Vector _position;
};
}