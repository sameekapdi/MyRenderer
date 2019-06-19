#pragma once
#include "stdafx.h"
#include "Vector.h"
#include <gdiplus.h>

namespace renderer
{
	class Vector; //forward declaration

	class Vertex
	{
	public://constructors
		Vertex(void);//defualt constructor
		Vertex(const Vertex&); //copy constructor
		~Vertex(void); //destructor
		
	public:
	    Vertex& operator=(Vertex& rhs);			// assignment operator

	public://fields
		Vector _verts;
		Vector _transVerts;
		Vector _vNormal;
		int _vCount;
		float _vR;
		float _vG;
		float _vB;
	};
}