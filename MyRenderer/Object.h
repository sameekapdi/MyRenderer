#include "stdafx.h"
#include "Polygon.h"
#include "Matrix.h"
#include "Vector.h"
#include "Vertex.h"
#include "Lights.h"
namespace renderer
{
	class Camera; //forward declaration
	class Object
	{
		public: //constructors
			Object(void); //default constructor
			Object(const Object&);//copy constructor
			~Object(void); //destructor
	public://operations
			void Transform(Matrix _mat, enum E_TRANSFORM_VERTS eTransformArgs);
			void Dehomogenise();
			void CalculateBackfaces(Vector &_camera);
			void Sort(void);
			void CalculatePolygonNormals();
			void CalculateVertexNormals();
			void CalculateLightingDirectionalFlat(Lights &_lights);
			void CalculateLightingPointFlat(Lights &_lights);
			void CalculateLightingDirectionalGouraud(Lights &_lights);
			void CalculateLightingPointGouraud(Lights &_lights);
			void CalculateLightingAmbient(Lights &_lights);
			void ClampRGB(Vector&);
			void SetDrawType(enum E_DRAW_TYPE dT);
			
		public://fields
			int _polycount;
			int _vertcount;
			Polygon _polys[4096]; //maximum polycount md2 loader supports
			Vertex _vertex[2048]; //maximum vertcount md2 loader supports
			E_DRAW_TYPE _drawType;
	};
	enum E_TRANSFORM_VERTS  // enum to pass in whether or not to transform verts locally or not
	{						
		_LOCAL_TO_TRANS,
		_TRANS
	};
	enum E_DRAW_TYPE		//enum to pass in the draw type to render
	{
		_VERTS,
		_WIREFRAME,
		_FLAT_GDIPLUS,
		_FLAT, 
		_GOURAUD
	};
}