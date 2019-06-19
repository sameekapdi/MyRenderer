#include "stdafx.h"
#include "Polygon.h"
namespace renderer
{
	Polygon::Polygon(void)
	{
		for (int i = 0;i<3;i++)						//initialise fields to zero
		{
			_indices[i] =0;							//initialise indices to zero
		}
		_backfacing = false;						//initialise backfacing to false
		_depth = 0;									//initialise depth to zero
		_pNormal = Vector(0.0f,0.0f,0.0f,0.0f);		//initialise normal to zero
		_pR = 000;									//initialise colour values to zero
		_pG = 000;
		_pB = 000;
	}
	Polygon::Polygon(const renderer::Polygon& poly)
	{
		for (int i = 0;i<3;i++)
		{
			_indices[i] = poly._indices[i];          //copy values into fields
		}
		_backfacing = poly._backfacing;				 
		_depth = poly._depth;
		_pR = poly._pR;
		_pG = poly._pG;
		_pB = poly._pB;
		_pNormal._x = poly._pNormal._x;
		_pNormal._y = poly._pNormal._y;
		_pNormal._z = poly._pNormal._z;
		_pNormal._w = poly._pNormal._w;

	}
	Polygon::~Polygon(void)
	{
		//Do nothing
	}
}
