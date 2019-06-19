#include "stdafx.h"
#include "Vector.h"
#include <gdiplus.h>
namespace renderer
{
	class Vector; //forward declaration
class Polygon
{
public: //constructors
	Polygon(void); //default constructor
	Polygon(const Polygon&);//copy constructor
	~Polygon(void); //destructor
public: //fields
	int _indices[3];
	bool _backfacing;
	float _depth;
	Vector _pNormal;
	float _pR;
	float _pG;
	float _pB;
};
}