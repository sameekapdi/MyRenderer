#include "StdAfx.h"
#include "Rasterizer.h"
#include <gdiplus.h>
#include <stdlib.h>
#include "Object.h"
#include "Vector.h"

namespace renderer
{
	Rasterizer::Rasterizer(void)
	{
		_fps =0;
		framecounter=0;					//initialise fields to zero
		lastframe=0;
		thisframe=0;
		for (int i=0;i<480;i++)
		{
			_scanlines[i]._startX = 640;		//initialise start values to their maximum
			_scanlines[i]._startR = 255;
			_scanlines[i]._startG = 255;
			_scanlines[i]._startB = 255;
			_scanlines[i]._endX = 0;			//initialise end values to their minimum
			_scanlines[i]._endR = 0;
			_scanlines[i]._endG = 0;
			_scanlines[i]._endB = 0;
		}
	}
	Rasterizer::Rasterizer(const Rasterizer& rast)
	{											//copy values into the fields
		_bitmap = rast._bitmap;
		_graphics = rast._graphics;
		_fps =rast._fps;
		framecounter=rast.framecounter;
		lastframe=rast.lastframe;
		thisframe=rast.thisframe;
		for (int i=0;i<480;i++)
		{
			_scanlines[i]._startX = rast._scanlines[i]._startX;
			_scanlines[i]._startR = rast._scanlines[i]._startR;
			_scanlines[i]._startG = rast._scanlines[i]._startG;
			_scanlines[i]._startB = rast._scanlines[i]._startB;
			_scanlines[i]._endX = rast._scanlines[i]._endX;
			_scanlines[i]._endR = rast._scanlines[i]._endR;
			_scanlines[i]._endG = rast._scanlines[i]._endG;
			_scanlines[i]._endB = rast._scanlines[i]._endB;
		}

	}
	Rasterizer::Rasterizer(int width, int height)
	{
		_bitmap = new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB);
		_graphics = new Gdiplus::Graphics(_bitmap);			//initialise fields
		_scanlines = new Scanlines[480];
		_fps =0;
		framecounter=0;
		lastframe=0;
		thisframe=0;
	}
	Rasterizer::~Rasterizer(void)
	{	
		//delete members
		delete _bitmap;
		delete _graphics;
		delete[] _scanlines;
	}
	void Rasterizer::Clear()
	{												//refresh the screen
		_graphics->Clear(Gdiplus::Color::Black);
	}
	void Rasterizer::DrawVerts(Object &_obj)
	{
		for (int i = 0;i<_obj._vertcount;i++)	//draw a pixel at every vert
		{
			_bitmap->SetPixel(_obj._vertex[i]._transVerts._x,_obj._vertex[i]._transVerts._y,Gdiplus::Color(255,255,255,255));
		}
		Gdiplus::FontFamily  fontFamily(L"Times New Roman");
		Gdiplus::Font        font(&fontFamily, 24, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
		Gdiplus::PointF      pointF(30.0f, 375.0f);
		Gdiplus::SolidBrush  solidBrush(Gdiplus::Color::White);
		_graphics->DrawString(L"Drawing Verts", 13, &font, pointF, &solidBrush); //display the draw type
	}
	void Rasterizer::DrawWireframe(Object& _obj)
	{
		Gdiplus::Pen      pen(Gdiplus::Color(255, 255, 0, 0));
		Gdiplus::PointF   v[3];

		for (int i = 0;i<_obj._polycount;i++)
		{
			if(_obj._polys[i]._backfacing == false)
			{
				v[0].X = _obj._vertex[ _obj._polys[i]._indices[0]]._transVerts._x; //find each x and y vert values of the polygon
				v[0].Y = _obj._vertex[ _obj._polys[i]._indices[0]]._transVerts._y;
				v[1].X = _obj._vertex[ _obj._polys[i]._indices[1]]._transVerts._x;
				v[1].Y = _obj._vertex[ _obj._polys[i]._indices[1]]._transVerts._y;
				v[2].X = _obj._vertex[ _obj._polys[i]._indices[2]]._transVerts._x;
				v[2].Y = _obj._vertex[ _obj._polys[i]._indices[2]]._transVerts._y;
				_graphics->DrawLine(&pen,v[0],v[1]);   //draw a line between each vert
				_graphics->DrawLine(&pen,v[1],v[2]);
				_graphics->DrawLine(&pen,v[2],v[0]);
			}
		}
		Gdiplus::FontFamily  fontFamily(L"Times New Roman");
		Gdiplus::Font        font(&fontFamily, 24, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
		Gdiplus::PointF      pointF(30.0f, 375.0f);
		Gdiplus::SolidBrush  solidBrush(Gdiplus::Color::White);
		_graphics->DrawString(L"Drawing Wireframe", 17, &font, pointF, &solidBrush); //display draw type
	}
	void Rasterizer::DrawSolidGDI(Object &_obj)
	{
		Gdiplus::PointF   v[3];

		for (int i = 0;i<_obj._polycount;i++)
		{
			if(_obj._polys[i]._backfacing == false)
			{
				v[0].X = _obj._vertex[ _obj._polys[i]._indices[0]]._transVerts._x; //find x and y values
				v[0].Y = _obj._vertex[ _obj._polys[i]._indices[0]]._transVerts._y;
				v[1].X = _obj._vertex[ _obj._polys[i]._indices[1]]._transVerts._x;
				v[1].Y = _obj._vertex[ _obj._polys[i]._indices[1]]._transVerts._y;
				v[2].X = _obj._vertex[ _obj._polys[i]._indices[2]]._transVerts._x;
				v[2].Y = _obj._vertex[ _obj._polys[i]._indices[2]]._transVerts._y;
				Gdiplus::SolidBrush brush(Gdiplus::Color(255,BYTE(_obj._polys[i]._pR),BYTE(_obj._polys[i]._pG),BYTE(_obj._polys[i]._pB)));
				_graphics->FillPolygon(&brush,v,3); //specify a brush and fill a polygon
			}
		}
		Gdiplus::FontFamily  fontFamily(L"Times New Roman");
		Gdiplus::Font        font(&fontFamily, 24, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
		Gdiplus::PointF      pointF(30.0f, 375.0f);
		Gdiplus::SolidBrush  solidBrush(Gdiplus::Color::White);
		_graphics->DrawString(L"Drawing Flat Shading GDI+", 25, &font, pointF, &solidBrush); //display the draw type
	}
	void Rasterizer::DrawFPS()
	{
		framecounter++;							//increase the framecounter
		thisframe = GetTickCount();				//get the currect tick count
		if ((thisframe-lastframe)>1000)			//check to see if 1000 milliseconds have passed = 1 second
		{
			_fps=framecounter;					//make the fps = framecount
			framecounter=0;						// reset framecounter
			lastframe = thisframe;				//make lastframe = this frame
		}
		Gdiplus::FontFamily  fontFamily(L"Times New Roman");
		Gdiplus::Font        font(&fontFamily, 24, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
		Gdiplus::PointF      pointF(30.0f, 10.0f);
		Gdiplus::SolidBrush  solidBrush(Gdiplus::Color::White);
		wchar_t fps[30];
		swprintf(fps,30,L"%.1f",_fps);								//convert float to string
		_graphics->DrawString(fps, -1, &font, pointF, &solidBrush); //display the string

	}
	void Rasterizer::DrawSolidFlat(Object &_obj)
	{
		Vertex   verts[3];

		for (int i = 0;i<_obj._polycount;i++)
		{
			if(_obj._polys[i]._backfacing == false)
			{											//find the verts for each polygon
				verts[0]._transVerts = _obj._vertex[ _obj._polys[i]._indices[0]]._transVerts;
				verts[1]._transVerts = _obj._vertex[ _obj._polys[i]._indices[1]]._transVerts;
				verts[2]._transVerts = _obj._vertex[ _obj._polys[i]._indices[2]]._transVerts;
				FillPolygonFlat(verts, Gdiplus::Color(255,BYTE(_obj._polys[i]._pR),BYTE(_obj._polys[i]._pG),BYTE(_obj._polys[i]._pB)));
			}
		}
		Gdiplus::FontFamily  fontFamily(L"Times New Roman");
		Gdiplus::Font        font(&fontFamily, 24, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
		Gdiplus::PointF      pointF(30.0f, 375.0f);
		Gdiplus::SolidBrush  solidBrush(Gdiplus::Color::White);
		_graphics->DrawString(L"Drawing Flat Shading", 20, &font, pointF, &solidBrush); //display the draw type

	}

	void Rasterizer::FillPolygonFlat(Vertex _verts[3], Gdiplus::Color &_Color)
	{
		int j=0;
		while (j<2)
		{
			for (int i=0;i<2;i++)						//sort the verts in terms of y
			{
				if (_verts[i]._transVerts._y>_verts[i+1]._transVerts._y)
				{
					Vertex temp = _verts[i+1];
					_verts[i+1] = _verts[i];
					_verts[i] = temp;
				}
			}
			j++;
		}
		int x;
		float top =_verts[0]._transVerts._y;			//find the top vert
		float middle = _verts[1]._transVerts._y;		//find the middle vert
		float bottom =_verts[2]._transVerts._y;			//find the bottom vert
		float d0 = middle - top;						//find distance between top and middle verts
		float d1 = bottom - middle;						//find distance between middle and bottom verts
		float d2 = bottom - top;						// find distance between top and bottom verts
		//FIND X VALUES!
		for (int j=0;j<480;j++)
		{
			_scanlines[j]._startX = 640;				//reset the scanline, x values
			_scanlines[j]._endX = 0;
		}

		ScanEdgesFlat(_verts[0],_verts[1]);				//find x values between top and middle,
		ScanEdgesFlat(_verts[1],_verts[2]);				//find x values between middle and bottom
		ScanEdgesFlat(_verts[0],_verts[2]);				//find x values between top and bottom
		DrawLinesFlat(top,bottom,_Color);				//draw line between top and bottm
	}
	void Rasterizer::ScanEdgesFlat(Vertex vert0, Vertex vert1)
	{
		Vertex v0;
		Vertex v1;

		if (vert0._transVerts._y < vert1._transVerts._y)
		{													//check to see if verts are wrong day around, prevents getting negative distances
			v0 = vert0;
			v1 = vert1;
		}
		else
		{
			v0 = vert1;
			v1 = vert0;
		}

		float dy = v1._transVerts._y - v0._transVerts._y; //find distance between top and bottom

		if (dy > 0)										// distance greater than zero, nullifies divide by zero
		{
			float dx = v1._transVerts._x - v0._transVerts._x; //distnace between x
			float gX = dx / dy;								  //gradient in terms of x	

			float tX = v0._transVerts._x;						// tempx store top vert

			for (int i = v0._transVerts._y; i <= v1._transVerts._y ; i++)
			{
				if (i > 0 && i < 480)							//going from top to bottom
				{												//and within the extents of our y value
					if (tX < _scanlines[i]._startX)				//test if tempx is smaller than max X
					{
						_scanlines[i]._startX = tX;				//store tempx in scanline
					}
					if (tX > _scanlines[i]._endX)				//test to see if x is larger than min X
					{
						_scanlines[i]._endX = tX;
					}
					if (_scanlines[i]._startX < 0) _scanlines[i]._startX = 0; //clamp x values
					if (_scanlines[i]._startX > 640)_scanlines[i]._startX = 640;

					if (_scanlines[i]._endX < 0) _scanlines[i]._endX = 0;
					if (_scanlines[i]._endX > 640) _scanlines[i]._endX = 640;
				}
				tX += gX; //increment temp x by the gradient
			}
		}
	}
	void Rasterizer::DrawLinesFlat(int top, int bottom, const Gdiplus::Color _Color)
	{
		for (int i=top;i<bottom+1;i++)
		{								//from top to bottom and for each scanline, set a pixel a colur
			for (int j=_scanlines[i]._startX;j<=_scanlines[i]._endX;j++)
			{
				_bitmap->SetPixel(j,i,_Color);
			}
		}
	}
	void Rasterizer::DrawSolidGouraud(Object &_obj)
	{
		Vertex	 vertex[3];

		for (int i = 0;i<_obj._polycount;i++)
		{
			if(_obj._polys[i]._backfacing == false)
			{
				vertex[0] = _obj._vertex[_obj._polys[i]._indices[0]];
				vertex[1] = _obj._vertex[_obj._polys[i]._indices[1]];
				vertex[2] = _obj._vertex[_obj._polys[i]._indices[2]];
				FillPolygonGouraud(vertex);
			}
		}
		Gdiplus::FontFamily  fontFamily(L"Times New Roman");
		Gdiplus::Font        font(&fontFamily, 24, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
		Gdiplus::PointF      pointF(30.0f, 375.0f);
		Gdiplus::SolidBrush  solidBrush(Gdiplus::Color::White);
		_graphics->DrawString(L"Drawing Gouraud", 15, &font, pointF, &solidBrush); //display draw type
	}
	void Rasterizer::FillPolygonGouraud(Vertex _verts[3])
	{
		int j=0;
		while (j<2)
		{
			for (int i=0;i<2;i++)					//sort verts in terms of y
			{
				if (_verts[i]._transVerts._y>_verts[i+1]._transVerts._y)
				{
					Vertex temp = _verts[i+1];
					_verts[i+1] = _verts[i];
					_verts[i] = temp;
				}
			}
			j++;
		}
		int x;
		float r, g, b;
		float top =_verts[0]._transVerts._y;
		float middle = _verts[1]._transVerts._y;
		float bottom =_verts[2]._transVerts._y;
		float d;
		float dy;
		float t=0;
		//FIND X VALUES!
		for (int j=0;j<480;j++)
		{
			_scanlines[j]._startX = 640;
			_scanlines[j]._endX = 0;
			_scanlines[j]._startR = 255;
			_scanlines[j]._startG = 255;	//reset values
			_scanlines[j]._startB = 255;
			_scanlines[j]._endR = 0;
			_scanlines[j]._endG = 0;
			_scanlines[j]._endB = 0;
		}

		ScanEdgesGouraud(_verts[0],_verts[1]);
		ScanEdgesGouraud(_verts[1],_verts[2]);
		ScanEdgesGouraud(_verts[0],_verts[2]);

		DrawLinesGouraud(top, bottom);
	}
	void Rasterizer::ScanEdgesGouraud(Vertex vert0, Vertex vert1)
	{
		Vertex v0;
		Vertex v1;

		if (vert0._transVerts._y < vert1._transVerts._y)
		{
			v0 = vert0;
			v1 = vert1;
		}
		else
		{
			v0 = vert1;
			v1 = vert0;
		}

		float dy = v1._transVerts._y - v0._transVerts._y;

		if (dy > 0)
		{
			float dx = v1._transVerts._x - v0._transVerts._x;
			float gX = dx / dy;

			float dR = v1._vR - v0._vR;			//find distance between colour values
			float dG = v1._vG - v0._vG;
			float dB = v1._vB - v0._vB;

			float gR = dR / dy;					//find gradient of colour values
			float gG = dG / dy;
			float gB = dB / dy;

			float tX = v0._transVerts._x;	//temp values = first vert values
			float tR = v0._vR;
			float tG = v0._vG;
			float tB = v0._vB;

			for (int i = v0._transVerts._y; i <= v1._transVerts._y ; i++)
			{
				if (i > 0 && i < 480)
				{
					if (tX < _scanlines[i]._startX)
					{
						_scanlines[i]._startX = tX;
						_scanlines[i]._startR = tR;
						_scanlines[i]._startG = tG;
						_scanlines[i]._startB = tB;
					}
					if (tX > _scanlines[i]._endX)
					{
						_scanlines[i]._endX = tX;
						_scanlines[i]._endR = tR;
						_scanlines[i]._endG = tG;
						_scanlines[i]._endB = tB;
					}
					if (_scanlines[i]._startX < 0) _scanlines[i]._startX = 0;
					if (_scanlines[i]._startX > 640)_scanlines[i]._startX = 640;

					if (_scanlines[i]._endX < 0) _scanlines[i]._endX = 0;
					if (_scanlines[i]._endX > 640) _scanlines[i]._endX = 640;
				}
				tX += gX;
				tR += gR;
				tG += gG;
				tB += gB;
			}
		}
	}
	void Rasterizer::DrawLinesGouraud(int top, int bottom)
	{
		float dx;
		float dr;
		float dg;			//store the distance between extents
		float db;
		float gr;
		float gg;			//store gradients of colours
		float gb;
		BYTE r =0;			//initialise colour values
		BYTE g=0;
		BYTE b=0;
		for (int i=top;i<bottom+1;i++)
		{
			for (int j=_scanlines[i]._startX;j<=_scanlines[i]._endX;j++)
			{
				dx = _scanlines[i]._endX - _scanlines[i]._startX;	//find distance between x values
				if (dx !=0)
				{
				dr = _scanlines[i]._endR - _scanlines[i]._startR;	//find distance between colour values
				dg = _scanlines[i]._endG - _scanlines[i]._startG;
				db = _scanlines[i]._endB - _scanlines[i]._startB;
				gr = dr / dx;										//find gradient of colours in terms of colour
				gg = dg / dx;
				gb = db / dx;
				r = _scanlines[i]._startR;					//assign colour values
				g = _scanlines[i]._startG;
				b = _scanlines[i]._startB;
				r += gr;									//increment colour value by gradient
				g += gg;
				b += gb;									
				if(r <0) {r =0;}
				else if(r>255) {r =255;}					//clamp colour values
				if(g <0) {g =0;}
				else if(g>255) {g =255;}
				if(b <0) {b =0;}
				else if(b>255) {b =255;}

				_bitmap->SetPixel(j,i, Gdiplus::Color(255,r,g,b)); //draw pixel
				}

			}
		}
	}
}