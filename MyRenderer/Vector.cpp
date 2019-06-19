#include "StdAfx.h"
#include "Vector.h"
#include <math.h>
namespace renderer
{
// default constructor
Vector::Vector(void)
{
	_x = 0.0f;							//initialise fields to zero
	_y = 0.0f;
	_z = 0.0f;
	_w = 0.0f;
}

// parameterised constructor
Vector::Vector(float x, float y , float z)
{
	_x = x;								//initialise fields to values
	_y = y;
	_z = z;
}
Vector::Vector(float x, float y , float z,float w)
{
	_x = x;								//initialise fields to values
	_y = y;
	_z = z;
	_w = w;
}

// copy constructor
Vector::Vector(const Vector& rhs)
{
	_x = rhs._x;						//copy values into the fields
	_y = rhs._y;
	_z = rhs._z;
	_w = rhs._w;
}

// destructor
Vector::~Vector(void)
{
	// do nothing
}

// assignment operator
Vector& Vector::operator=(Vector& rhs)
{	
	this->_x = rhs._x;						//assign values to the fields
	this->_y = rhs._y;
	this->_z = rhs._z;
	this->_w = rhs._w;
	return *this;
}

// ARITHMETIC OPERATIONS //------------------------------

Vector Vector::Add(Vector rhs)
{
    _x = _x + rhs._x;
	_y = _y + rhs._y;						//add two vectors
	_z = _z + rhs._z;
	_w = _w + rhs._w;
	return Vector(_x,_y,_z,_w);
}

Vector Vector::Subtract(Vector rhs)
{
	_x = _x - rhs._x;
	_y = _y - rhs._y;						//subtract two vectors
	_z = _z - rhs._z;
	_w = _w - rhs._w;
	return Vector(_x,_y,_z,_w);
}

Vector Vector::Multiply(float scalar)
{										//multiply a vector by a scalar
	_x = _x*scalar;
	_y = _y*scalar;
	_z = _z*scalar;
	_w = _w*scalar;
	return Vector(_x,_y,_z,_w);
}
Vector Vector::Divide(float scalar)
{										//divide a vector by a scalar
	_x = _x/scalar;
	_y = _y/scalar;
	_z = _z/scalar;
	_w = _w/scalar;
	return Vector(_x,_y,_z,_w);
}
// END ARITHMETIC OPERATIONS //--------------------------

// OTHER OPERATIONSN //----------------------------------

float Vector::Magnitude()
{
	float mag = sqrt((_x*_x)+(_y*_y)+(_z*_z));			//find the distance of a vector
	return mag;
}

void  Vector::MakeUnit()
{
	Vector vec(_x,_y,_z);							//normalise a vector
	vec = vec.Divide(vec.Magnitude());
	_x = vec._x;
	_y = vec._y;
	_z = vec._z;
	_w = 1;
}
float Vector::DotProduct(Vector v)
{
	float dotP = ((_x*v._x)+(_y*v._y)+(_z*v._z));		//find the angle between two vectors
	return dotP;
}

Vector Vector::CrossProduct(Vector v)
{
	Vector vec(0.0f,0.0f,0.0f,0.0f);				//find the normal of two vectors
	vec._x = ((_y*v._z)-(_z*v._y));
	vec._y = ((_x*v._z)-(_z*v._x));
	vec._z = ((_x*v._y)-(_y*v._x));
	vec._w = 0;
	return vec;
}

// END OTHER OPERATIONS //--------------------------------

// print
std::ostream& operator<<(std::ostream& out, const Vector& vec)
{
	return out << "<" << vec._x << ", " << vec._y << ", " << vec._z << ", "<<vec._w << ">" << std::endl;
	//float n;
	//if (vec._w != n)
	//{
	//return out << "<" << vec._x << ", " << vec._y << ", " << vec._z << ", "<<vec._w << ">" << std::endl;
	//}
	//else
	//{
	//return out << "<" << vec._x << ", " << vec._y << ", " << vec._z << ">" << std::endl;
	//}
}
}