#pragma once
#include <iostream>		// this is included only for std::ostream in order to implement operator<<()
namespace renderer
{
class Vector
{
public:
	Vector(void);											// default constructor
	Vector(float x, float y, float z);						// parameterised constructor
	Vector(float x, float y, float z, float w);
	Vector(const Vector& rhs);								// copy constructor
	~Vector(void);											// destructor

	// operators
public:
	Vector& operator=(Vector& rhs);							// assignment operator

	// arithmetic operations
public:
	Vector Add(Vector rhs);
	Vector Subtract(Vector rhs);
	Vector Multiply(float scalar);
	Vector Divide(float scalar);

	// other operations
public:
	float  Magnitude();
	void   MakeUnit();
	float  DotProduct(Vector rhs);
	Vector CrossProduct(Vector rhs);

public:
	// output operator implemented for printing
	friend std::ostream& operator<<(std::ostream&, const Vector&);

	// fields
public:
	float _x;
	float _y;
	float _z;
	float _w;
};
}

