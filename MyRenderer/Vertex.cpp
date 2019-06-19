#include "stdafx.h"
#include "Vertex.h"
#include "Vector.h"
#include <gdiplus.h>

namespace renderer
{
	Vertex::Vertex(void)
	{
		_verts = Vector(0.0f,0.0f,0.0f,0.0f);				//initialise fields to zero
		_transVerts = Vector(0.0f,0.0f,0.0f,0.0f);
		_vNormal = Vector(0.0f,0.0f,0.0f,0.0f);
		_vCount = 0;
		_vR = 000;
		_vG = 000;
		_vB = 000;
	}

	Vertex::Vertex(const renderer::Vertex & vertex)
	{
		_verts._x = vertex._verts._x;					//copy values to fields
		_verts._y = vertex._verts._y;
		_verts._z = vertex._verts._z;
		_verts._w = vertex._verts._w;
		_transVerts._x = vertex._transVerts._x;
		_transVerts._y = vertex._transVerts._y;
		_transVerts._z = vertex._transVerts._z;
		_transVerts._w = vertex._transVerts._w;
		_vNormal._x = vertex._vNormal._x;
		_vNormal._y = vertex._vNormal._y;
		_vNormal._z = vertex._vNormal._z;
		_vNormal._w = vertex._vNormal._w;
		_vCount = vertex._vCount;
		_vR = vertex._vR;
		_vG = vertex._vG;
		_vB = vertex._vB;
	}
	Vertex::~Vertex(void)
	{

	}
	Vertex& Vertex::operator=(Vertex& rhs)
	{												//assign values to fields
	this->_verts._x = rhs._verts._x;
	this->_verts._y = rhs._verts._y;
	this->_verts._z = rhs._verts._z;
	this->_verts._w = rhs._verts._w;
	this->_transVerts._x = rhs._transVerts._x;
	this->_transVerts._y = rhs._transVerts._y;
	this->_transVerts._z = rhs._transVerts._z;
	this->_transVerts._w = rhs._transVerts._w;
	this->_vNormal._x = rhs._vNormal._x;
	this->_vNormal._y = rhs._vNormal._y;
	this->_vNormal._z = rhs._vNormal._z;
	this->_vNormal._w = rhs._vNormal._w;
	this->_vCount = rhs._vCount;
	this->_vR = rhs._vR;
	this->_vG = rhs._vG;
	this->_vB = rhs._vB;
	return *this;
	}
}