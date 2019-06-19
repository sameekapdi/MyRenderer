#pragma once
#include <gdiplus.h>

namespace renderer
{
	struct Scanlines	//struct to hold scanline information
	{
		int _startX;
		int _endX;
		float _startR;
		float _startG;
		float _startB;
		float _endR;
		float _endG;
		float _endB;
	};
// forward declaration
class Vector;
class Vertex;
class Object;

	class Rasterizer
	{
	public: //constructors
		Rasterizer(void); //default constructor
		Rasterizer(const Rasterizer&); //copy constructor
		Rasterizer(int width, int height); //parameterised constructor
		~Rasterizer(void); //destructor
	public: //operations
		void Clear();
		
		void DrawVerts(Object &_object); //Draw verts
		void DrawWireframe(Object &_object); //Draw wireframe
		void DrawSolidGDI(Object &_object); //Draw flat shading with gdiplus
		void DrawFPS(); //Draw FPS
		
		//Draw flat shading
		void DrawSolidFlat(Object &_object);
		void FillPolygonFlat(Vertex _verts[3], Gdiplus::Color &_Color);
		void ScanEdgesFlat(Vertex vert0, Vertex vert1);
		void DrawLinesFlat(int top, int bottom, const Gdiplus::Color _Color);
		
		//Draw gouraud shading
		void DrawSolidGouraud(Object &_object);
		void FillPolygonGouraud(Vertex _verts[3]);
		void ScanEdgesGouraud(Vertex vert0, Vertex vert1);
		void DrawLinesGouraud(int top, int bottom);

	public: //fields
		Gdiplus::Bitmap* _bitmap;
		Gdiplus::Graphics* _graphics;
		Scanlines* _scanlines;
		float _fps;
		int framecounter;
		int lastframe;
		int thisframe;
	};
}