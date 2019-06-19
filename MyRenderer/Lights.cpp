#include "stdafx.h"
#include "Lights.h"

namespace renderer
{
	Lights::Lights(void)
	{												//Initialise fields to zero
	_La = Gdiplus::Color(000,000,000,000);
	_ka_red = 0;
	_ka_green = 0;
	_ka_blue = 0;
	_Ld = Gdiplus::Color(000,000,000,000);
	_kd_red = 0;
	_kd_green = 0;
	_kd_blue = 0;
	_source = Vector(0.0f,0.0f,0.0f,0.0f);
	}
	Lights::Lights(Gdiplus::Color I, Gdiplus::Color La,	float ka_red, float ka_green, float ka_blue, 
					Gdiplus::Color Ld, float kd_red, float kd_green, float kd_blue, Vector source)
	{												//assign given values to fields
	_I = I;
	_La = La;
	_ka_red = ka_red;
	_ka_green = ka_green;
	_ka_blue = ka_blue;
	_Ld = Ld;
	_kd_red = kd_red;
	_kd_green = kd_green;
	_kd_blue = kd_blue;
	_source = Vector(source._x,source._y,source._z,source._w);
	}
	Lights::Lights(const renderer::Lights & light)
	{												//copy values to fields
	_La = light._La;
	_ka_red = light._ka_red;
	_ka_green = light._ka_green;
	_ka_blue = light._ka_blue;
	_Ld = light._Ld;
	_kd_red = light._kd_red;
	_kd_green = light._kd_green;
	_kd_blue = light._kd_blue;
	_source = Vector(light._source._x,light._source._y,light._source._z,light._source._w);
	}
	Lights::~Lights(void)
	{
		//Do nothing
	}
};