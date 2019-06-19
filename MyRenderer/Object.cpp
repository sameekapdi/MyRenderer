#include "stdafx.h"
#include "Object.h"
#include "Camera.h"
#include <vector>
#include <algorithm>

namespace renderer
{
	Object::Object(void)
	{
		_polycount = 0;								//initialise polycount to zero
		_vertcount = 0;								//initialise vertcount to zero
		_drawType = _FLAT_GDIPLUS;					//Initialise to flat shading with gdiplus

		for(int i = 0;i<2048;i++)
		{					
			_polys[i] = Polygon();					//initialise polygon
			_vertex[i] = Vertex();					//initialise vertices
		}
		for(int i = 2048;i<4096;i++)
		{
			_polys[i] = Polygon();					//initialise remaining polygons
		}

	}
	Object::Object(const renderer::Object & object)
	{
		_polycount = object._polycount;				//copy polycount
		_vertcount = object._vertcount;				//copy vertcount
	}
	Object::~Object(void)
	{}
	void Object::Transform(renderer::Matrix _mat, renderer::E_TRANSFORM_VERTS eTransformArgs)
	{
		if (eTransformArgs ==0)					//tranform from local space to world space
		{
			for (int i = 0; i<_vertcount;i++)
			{
				Matrix::Transform(_mat, _vertex[i]._verts,_vertex[i]._transVerts);
				_vertex[i]._verts._x = _vertex[i]._transVerts._x;
				_vertex[i]._verts._y = _vertex[i]._transVerts._y;	//By making the verts = the transformed verts
				_vertex[i]._verts._z = _vertex[i]._transVerts._z;	//it adds the rotation values
				_vertex[i]._verts._w = _vertex[i]._transVerts._w;	//so the object keeps spinning
			}
		}
		else									//transform from world to camera to perspective to screen space 
		{
			for (int i = 0; i<_vertcount;i++)
			{
				Matrix::Transform(_mat, _vertex[i]._transVerts,_vertex[i]._transVerts);
			}
		}
	}
	void Object::Dehomogenise()
	{
		for (int i = 0; i<_polycount;i++)			//flatten w to one,
		{											//moves from 4d to 3d
			if (_polys[i]._backfacing ==false)
			{
				_vertex[_polys[i]._indices[0]]._transVerts.Divide(_vertex[_polys[i]._indices[0]]._transVerts._w);
				_vertex[_polys[i]._indices[1]]._transVerts.Divide(_vertex[_polys[i]._indices[1]]._transVerts._w);
				_vertex[_polys[i]._indices[2]]._transVerts.Divide(_vertex[_polys[i]._indices[2]]._transVerts._w);
			}
		}
	}
	void Object::CalculateBackfaces(Vector& cam)
	{											//calculate back faces for culling
		for (int i =0; i<_polycount;i++)
		{
			_polys[i]._backfacing = false;		//reset bool
			Vector eye = ((Vector(cam._x,cam._y,cam._z,cam._w)).Subtract(Vector(_vertex[_polys[i]._indices[0]]._transVerts._x,_vertex[_polys[i]._indices[0]]._transVerts._y,_vertex[_polys[i]._indices[0]]._transVerts._z,_vertex[_polys[i]._indices[0]]._transVerts._w)));
			eye.MakeUnit();						//normalise
			float dp = eye.DotProduct(_polys[i]._pNormal);

			if (dp<0)
			{
				_polys[i]._backfacing = true;
			}
		}
	}

	void Object::ClampRGB(Vector& RGB)
	{									//clamp RGB values from overflowing and underflowing
		if(RGB._x <0)
		{
			RGB._x =0;
		}
		else if(RGB._x>255)
		{
			RGB._x =255;
		}
		if(RGB._y <0)
		{
			RGB._y =0;
		}
		else if(RGB._y>255)
		{
			RGB._y =255;
		}
		if(RGB._z <0)
		{
			RGB._z =0;
		}
		else if(RGB._z>255)
		{
			RGB._z =255;
		}
	}
	void Object::SetDrawType(enum E_DRAW_TYPE dT)
	{
		_drawType = dT;							//set the draw type
	}
	// Return whether first element is greater than the second
	bool UDgreater ( Polygon& elem1, Polygon& elem2 )
	{
		return elem1._depth > elem2._depth;
	}

	void Object::Sort()
	{												//sort polygons by the depth, z value
		for (int i = 0; i<_polycount;i++)
		{
			_polys[i]._depth =(((_vertex[_polys[i]._indices[0]]._transVerts._z)+(_vertex[_polys[i]._indices[1]]._transVerts._z)+(_vertex[_polys[i]._indices[2]]._transVerts._z))/3);
		}		//find the average depth from the z values of each vert

		std::sort(_polys,_polys+_polycount, UDgreater); //sort the average depth
	}
	void Object::CalculatePolygonNormals()
	{											//calculate polygon normals
		for (int i=0;i<_polycount;i++)
		{										
			Vector v0(_vertex[_polys[i]._indices[0]]._transVerts._x,_vertex[_polys[i]._indices[0]]._transVerts._y,_vertex[_polys[i]._indices[0]]._transVerts._z,_vertex[_polys[i]._indices[0]]._transVerts._w);
			Vector v1(_vertex[_polys[i]._indices[1]]._transVerts._x,_vertex[_polys[i]._indices[1]]._transVerts._y,_vertex[_polys[i]._indices[1]]._transVerts._z,_vertex[_polys[i]._indices[1]]._transVerts._w);
			Vector v2(_vertex[_polys[i]._indices[2]]._transVerts._x,_vertex[_polys[i]._indices[2]]._transVerts._y,_vertex[_polys[i]._indices[2]]._transVerts._z,_vertex[_polys[i]._indices[2]]._transVerts._w);
			Vector a = v1.Subtract(v0);				//find the vecotr between the verts
			Vector b = v2.Subtract(v0);
			_polys[i]._pNormal = b.CrossProduct(a);	//the the surface normal
			_polys[i]._pNormal.MakeUnit();			//normalise the vector
		}
	}

	void Object::CalculateVertexNormals()
	{												//calculate the vertex normals
		for (int i=0;i<_polycount;i++)
		{											//go through each polygon and add each normal the the vertex normal
			_vertex[_polys[i]._indices[0]]._vNormal.Add(_polys[i]._pNormal);
			_vertex[_polys[i]._indices[0]]._vCount++;	//increment the number of times a normal is added to
			_vertex[_polys[i]._indices[1]]._vNormal.Add(_polys[i]._pNormal);
			_vertex[_polys[i]._indices[1]]._vCount++;
			_vertex[_polys[i]._indices[2]]._vNormal.Add(_polys[i]._pNormal);
			_vertex[_polys[i]._indices[2]]._vCount++;
		}

		for (int j = 0;j<_vertcount;j++)
		{											//go through each vert and divide the normal by
			_vertex[j]._vNormal.Divide(float(_vertex[j]._vCount)); //how many times it was added to.
			_vertex[j]._vNormal.MakeUnit();			//normalise the result
		}
	}
	void Object::CalculateLightingDirectionalFlat(Lights &_lights)
	{
		Vector totalRGB;							//holds the final r g b values
		Vector tempRGB;								//holds the r g b values whilst we're calculating
		float lamb;									//holds the dot product between surface normal and light source
		float iRed;									//stores the r value of the total illumination
		float iGreen;								//stores the g value of the total illumination
		float iBlue;								//stores the b value of the total illumination

		for (int i=0;i<_polycount;i++)				//for each polygon
		{

			totalRGB = Vector(0.0f,0.0f,0.0f);		//reset total rgb
			tempRGB._x = _lights._Ld.GetRed();		//get red value of our diffuse light
			tempRGB._y = _lights._Ld.GetGreen();	//get green value of our diffuse light
			tempRGB._z = _lights._Ld.GetBlue();		//get blue value of our diffuse light
			tempRGB._x *= _lights._kd_red;			//times red value by its reflectance co efficient
			tempRGB._y *= _lights._kd_green;		//times green value by its reflectance co efficient
			tempRGB._z *= _lights._kd_blue;			//times blue value by its reflectance co efficient
			iRed = _lights._I.GetRed();				//get the red value of total illumination
			iGreen =_lights._I.GetGreen();			//get green value of total illumination
			iBlue =_lights._I.GetBlue();			//get blue value of total illumination
													//calculate vector between light course and polygon
			Vector l = ((Vector(_lights._source._x,_lights._source._y,_lights._source._z,_lights._source._w)).Subtract(Vector(_vertex[_polys[i]._indices[0]]._transVerts._x,_vertex[_polys[i]._indices[0]]._transVerts._y,_vertex[_polys[i]._indices[0]]._transVerts._z,_vertex[_polys[i]._indices[0]]._transVerts._w)));
			float d = l.Magnitude();				//find the distance
			l.MakeUnit();							//normalise vector
			lamb = l.DotProduct(_polys[i]._pNormal);//find the angle between vectors
			if (lamb <0)							//clamp dot product between 0 and 1
			{
				lamb = 0;
			}
			else if(lamb >1)
			{
				lamb = 1;
			}
			tempRGB.Multiply(lamb);					//attenuate colour value
			//add light contribution
			tempRGB = tempRGB.Add(Vector(_lights._I.GetRed(),_lights._I.GetGreen(),_lights._I.GetBlue(),0.0f));
			totalRGB = Vector(tempRGB._x,tempRGB._y,tempRGB._z);	//store total colour value
			ClampRGB(totalRGB);						//value between 0 and 255
			_polys[i]._pR = totalRGB._x;
			_polys[i]._pG = totalRGB._y;
			_polys[i]._pB = totalRGB._z;			//store colour values in polygon
		}			
	}
	void Object::CalculateLightingPointFlat(Lights &_lights)
	{
		Vector totalRGB;
		Vector tempRGB;
		for (int i=0;i<_polycount;i++)
		{
			totalRGB = Vector(0.0f,0.0f,0.0f);
			tempRGB._x = _lights._Ld.GetRed();
			tempRGB._y = _lights._Ld.GetGreen();
			tempRGB._z = _lights._Ld.GetBlue();
			tempRGB._x *= _lights._kd_red;
			tempRGB._y *= _lights._kd_green;
			tempRGB._z *= _lights._kd_blue;
			Vector l = ((Vector(_lights._source._x,_lights._source._y,_lights._source._z,_lights._source._w)).Subtract(Vector(_vertex[_polys[i]._indices[0]]._transVerts._x,_vertex[_polys[i]._indices[0]]._transVerts._y,_vertex[_polys[i]._indices[0]]._transVerts._z,_vertex[_polys[i]._indices[0]]._transVerts._w)));
			float d = l.Magnitude();
			float isl = (1/(1.0f+(1.0f*d)+(1.0f*d*d)));		//attenuate by applying inverse square law
			l.MakeUnit();
			float lamb = l.DotProduct(_polys[i]._pNormal);
			if (lamb <0)
			{
				lamb = 0;
			}
			else if(lamb >1)
			{
				lamb = 1;
			}
			tempRGB.Multiply(lamb*isl);
			//add lighting contribution
			totalRGB = Vector(tempRGB._x+_polys[i]._pR,tempRGB._y+_polys[i]._pG,tempRGB._z+_polys[i]._pB);
			ClampRGB(totalRGB);

			_polys[i]._pR = totalRGB._x;
			_polys[i]._pG = totalRGB._y;	//store colour value in polygon
			_polys[i]._pB = totalRGB._z;
		}
	}
	void Object::CalculateLightingDirectionalGouraud(Lights &_lights)
	{
		Vector totalRGB(0.0f,0.0f,0.0f,0.0f);
		Vector tempRGB(0.0f,0.0f,0.0f,0.0f);
		float iRed;
		float iGreen;
		float iBlue;
		float lamb;

		for (int i=0;i<_vertcount;i++)				//for each vertex
		{

			totalRGB = Vector(0.0f,0.0f,0.0f,0.0f);
			tempRGB._x = _lights._Ld.GetRed();
			tempRGB._y = _lights._Ld.GetGreen();
			tempRGB._z = _lights._Ld.GetBlue();
			tempRGB._x *= _lights._kd_red;
			tempRGB._y *= _lights._kd_green;
			tempRGB._z *= _lights._kd_blue;
			iRed = _lights._I.GetRed();
			iGreen =_lights._I.GetGreen();
			iBlue =_lights._I.GetBlue();
			//find the vector between light source and vertex normal
			Vector l = ((Vector(_lights._source._x,_lights._source._y,_lights._source._z,_lights._source._w)).Subtract(Vector(_vertex[i]._transVerts._x,_vertex[i]._transVerts._y,_vertex[i]._transVerts._z,_vertex[i]._transVerts._w)));
			l.MakeUnit();
			//find the angle between the vector and the vertex normal
			lamb = l.DotProduct(_vertex[i]._vNormal);
			if (lamb <0)
			{
				lamb = 0;
			}
			else if(lamb >1)
			{
				lamb = 1;
			}
			tempRGB.Multiply(lamb);
			tempRGB = tempRGB.Add(Vector(_lights._I.GetRed(),_lights._I.GetGreen(),_lights._I.GetBlue(),0.0f));
			totalRGB = Vector(tempRGB._x,tempRGB._y,tempRGB._z);
			ClampRGB(totalRGB);
			_vertex[i]._vR= totalRGB._x;
			_vertex[i]._vG= totalRGB._y;
			_vertex[i]._vB = totalRGB._z;			//store the colour values in the vertex
		}
	}
	void Object::CalculateLightingPointGouraud(Lights &_lights)
	{
		Vector totalRGB;
		Vector tempRGB;
		for (int i=0;i<_vertcount;i++)
		{
			totalRGB = Vector(0.0f,0.0f,0.0f);
			tempRGB._x = _lights._Ld.GetRed();
			tempRGB._y = _lights._Ld.GetGreen();
			tempRGB._z = _lights._Ld.GetBlue();
			tempRGB._x *= _lights._kd_red;
			tempRGB._y *= _lights._kd_green;
			tempRGB._z *= _lights._kd_blue;

			Vector l = ((Vector(_lights._source._x,_lights._source._y,_lights._source._z,_lights._source._w)).Subtract(Vector(_vertex[i]._transVerts._x,_vertex[i]._transVerts._y,_vertex[i]._transVerts._z,_vertex[i]._transVerts._w)));
			float d = l.Magnitude();
			float isl = (1/(0.01+(0.01*d)+(0.01*d*d)));
			l.MakeUnit();
			float lamb = l.DotProduct(_vertex[i]._vNormal);
			if (lamb <0)
			{
				lamb = 0;
			}
			else if(lamb >1)
			{
				lamb = 1;
			}
			tempRGB.Multiply(lamb*isl);
			totalRGB = Vector(tempRGB._x+_vertex[i]._vR,tempRGB._y+_vertex[i]._vG,tempRGB._z+_vertex[i]._vB);
			ClampRGB(totalRGB);

			_vertex[i]._vR = totalRGB._x;
			_vertex[i]._vG = totalRGB._y;
			_vertex[i]._vB = totalRGB._z;
		}
	}
	void Object::CalculateLightingAmbient(Lights &_lights)
	{
		Vector totalRGB;
		Vector tempRGB;
		tempRGB._x = _lights._La.GetRed();			//find the red value of ambient light
		tempRGB._y = _lights._La.GetGreen();		//find green value of ambient light
		tempRGB._z = _lights._La.GetBlue();			//find blue value of ambient light
		tempRGB._x *= _lights._ka_red;				//multiply red value with red ambient coefficient
		tempRGB._y *= _lights._ka_green;			//multiply green value with green ambient coefficient
		tempRGB._z *= _lights._ka_blue;				//multiply blue value with blue ambient coefficient
		totalRGB = Vector(tempRGB._x,tempRGB._y,tempRGB._z);	//store colour values
		ClampRGB(totalRGB);							//clamp colour values
		//set the total illumination
		_lights._I = Gdiplus::Color(255,BYTE(totalRGB._x),BYTE(totalRGB._y),BYTE(totalRGB._z));
	}
}