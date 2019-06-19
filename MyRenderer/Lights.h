#include "stdafx.h"
#include "Vector.h"
#include <gdiplus.h>
namespace renderer
{
	class Lights
	{
	public://constructors
		Lights(void); //default constructor
		Lights(Gdiplus::Color I,Gdiplus::Color La, float ka_red, float ka_green, float ka_blue, //parameterised
			   Gdiplus::Color Ld, float kd_red, float kd_green, float kd_blue,	Vector source); //constructor
		Lights(const Lights&); //copy constructor
		~Lights(void);//destructor
	public://fields
		Gdiplus::Color _I;
		Gdiplus::Color _La;
		float _ka_red;
		float _ka_green;
		float _ka_blue;
		Gdiplus::Color _Ld;
		float _kd_red;
		float _kd_green;
		float _kd_blue;
		Vector _source;

	};
}