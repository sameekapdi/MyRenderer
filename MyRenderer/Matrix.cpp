#include "StdAfx.h"
#include "Vector.h"
#include "Matrix.h"
#include <math.h>


namespace renderer
{
Matrix::Matrix(void)
{													
	//Initialise fields to zero
	for (int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			_m[i][j] = 0.0f;
		}
	}
}
Matrix::Matrix(float a00, float a01, float a02, float a03,
			   float a10, float a11, float a12, float a13,
			   float a20, float a21, float a22, float a23,
			   float a30, float a31, float a32, float a33)
{													
	//assign given values to the fields
	_m[0][0] = a00;	_m[0][1] = a01;	_m[0][2] = a02;	_m[0][3] = a03;
	_m[1][0] = a10;	_m[1][1] = a11;	_m[1][2] = a12;	_m[1][3] = a13;
	_m[2][0] = a20;	_m[2][1] = a21;	_m[2][2] = a22;	_m[2][3] = a23;
	_m[3][0] = a30;	_m[3][1] = a31;	_m[3][2] = a32;	_m[3][3] = a33;
}
Matrix::Matrix(const Matrix& mat)
{													
	//copy values to the fields
	_m[0][0] = mat._m[0][0];	_m[0][1] = mat._m[0][1];	_m[0][2] = mat._m[0][2];	_m[0][3] = mat._m[0][3];
	_m[1][0] = mat._m[1][0];	_m[1][1] = mat._m[1][1];	_m[1][2] = mat._m[1][2];	_m[1][3] = mat._m[1][3];
	_m[2][0] = mat._m[2][0];	_m[2][1] = mat._m[2][1];	_m[2][2] = mat._m[2][2];	_m[2][3] = mat._m[2][3];
	_m[3][0] = mat._m[3][0];	_m[3][1] = mat._m[3][1];	_m[3][2] = mat._m[3][2];	_m[3][3] = mat._m[3][3];
}
Matrix::~Matrix(void)
{
	//Do nothing
}
Matrix& Matrix::operator = (Matrix& mat)
{													
	//assignment operator
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			this->_m[i][j] = mat._m[i][j];
		}
	}
	return *this;
}
void Matrix::Add( const Matrix& mat1, const Matrix& mat2, Matrix& result)
{													
	//Add two matrices together
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			result._m[i][j] = mat1._m[i][j] + mat2._m[i][j];
		}
	}
}

void Matrix::Subtract( const Matrix& mat1, const Matrix& mat2, Matrix& result)
{													
	//subtract two matrices from each other
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			result._m[i][j] = mat1._m[i][j] - mat2._m[i][j];
		}
	}
}
void Matrix::Multiply( const Matrix& mat1, float scalar, Matrix& result)
{													
	//Multiply a matrix by a scalar
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			result._m[i][j] = scalar*(mat1._m[i][j]);
		}
	}
}
void Matrix::Multiply( const Matrix& mat1, const Matrix& mat2, Matrix& result)
{													
	//multiply a matrix with another matrix
	for(int i=0;i<4;i++)
	{
		for(int j = 0;j<4;j++)
		{
			result._m[i][j]= (mat1._m[i][0]*mat2._m[0][j])+(mat1._m[i][1]*mat2._m[1][j])+(mat1._m[i][2]*mat2._m[2][j])+(mat1._m[i][3]*mat2._m[3][j]);
		}
	}

}
void Matrix::Transform( const Matrix& mat1, const Vector& vec1, Vector& result)
{													//multiply a matrix with a vector
	result._x = (mat1._m[0][0]*vec1._x)+(mat1._m[0][1]*vec1._y)+(mat1._m[0][2]*vec1._z)+(mat1._m[0][3]*vec1._w);
	result._y = (mat1._m[1][0]*vec1._x)+(mat1._m[1][1]*vec1._y)+(mat1._m[1][2]*vec1._z)+(mat1._m[1][3]*vec1._w);
	result._z = (mat1._m[2][0]*vec1._x)+(mat1._m[2][1]*vec1._y)+(mat1._m[2][2]*vec1._z)+(mat1._m[2][3]*vec1._w);
	result._w = (mat1._m[3][0]*vec1._x)+(mat1._m[3][1]*vec1._y)+(mat1._m[3][2]*vec1._z)+(mat1._m[3][3]*vec1._w);
}

void Matrix::Rotate(Matrix& result, float anglex, float angley, float anglez)
{													
	//rotate a matrix
		Matrix temp;
		Matrix rotX(1.0f,0.0f,0.0f,0.0f,
				0.0f, cos(anglex),-sin(anglex),0.0f,
				0.0f, sin(anglex), cos(anglex),0.0f,
				0.0f,0.0f,0.0f,1.0f);				//find X rotation

		Matrix rotY(cos(angley),0.0f,sin(angley),0.0f,
				0.0f, 1.0f,0.0f,0.0f,
				-sin(angley), 0.0f, cos(angley),0.0f,
				0.0f,0.0f,0.0f,1.0f);				//find Y rotation
		Matrix::Multiply(rotX,rotY,temp);			//combine the X and Y rotation
		

		Matrix rotZ(cos(anglez),-sin(anglez),0.0f,0.0f,
				sin(anglez), cos(anglez),0.0f,0.0f,
				0.0f, 0.0f, 1.0f,0.0f,
				0.0f,0.0f,0.0f,1.0f);				//find Z rotation
		Matrix::Multiply(temp,rotZ,result);			//combine Z with XY rotation

}
void Matrix::Transpose(const Matrix& mat1, Matrix& result)
{													
	//Transpose a matrix
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result._m[i][j] = mat1._m[j][i];
		}
	}

}
void Matrix::Inverse(const Matrix& mat1, Matrix& result)//Find the inverse of a matrix
{				
	//Formula edited from:
				//http://www.euclideanspace.com/maths/algebra/matrix/functions/inverse/fourD/index.htm

	Matrix inverse(0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f);
   inverse._m[0][0] = mat1._m[1][2]*mat1._m[2][3]*mat1._m[3][1] - mat1._m[1][3]*mat1._m[2][2]*mat1._m[3][1] + mat1._m[1][3]*mat1._m[2][1]*mat1._m[3][2] - mat1._m[1][1]*mat1._m[2][3]*mat1._m[3][2] - mat1._m[1][2]*mat1._m[2][1]*mat1._m[3][3] + mat1._m[1][1]*mat1._m[2][2]*mat1._m[3][3];
   inverse._m[0][1] = mat1._m[0][3]*mat1._m[2][2]*mat1._m[3][1] - mat1._m[0][2]*mat1._m[2][3]*mat1._m[3][1] - mat1._m[0][3]*mat1._m[2][1]*mat1._m[3][2] + mat1._m[0][1]*mat1._m[2][3]*mat1._m[3][2] + mat1._m[0][2]*mat1._m[2][1]*mat1._m[3][3] - mat1._m[0][1]*mat1._m[2][2]*mat1._m[3][3];
   inverse._m[0][2] = mat1._m[0][2]*mat1._m[1][3]*mat1._m[3][1] - mat1._m[0][3]*mat1._m[1][2]*mat1._m[3][1] + mat1._m[0][3]*mat1._m[1][1]*mat1._m[3][2] - mat1._m[0][1]*mat1._m[1][3]*mat1._m[3][2] - mat1._m[0][2]*mat1._m[1][1]*mat1._m[3][3] + mat1._m[0][1]*mat1._m[1][2]*mat1._m[3][3];
   inverse._m[0][3] = mat1._m[0][3]*mat1._m[1][2]*mat1._m[2][1] - mat1._m[0][2]*mat1._m[1][3]*mat1._m[2][1] - mat1._m[0][3]*mat1._m[1][1]*mat1._m[2][2] + mat1._m[0][1]*mat1._m[1][3]*mat1._m[2][2] + mat1._m[0][2]*mat1._m[1][1]*mat1._m[2][3] - mat1._m[0][1]*mat1._m[1][2]*mat1._m[2][3];
   inverse._m[1][0] = mat1._m[1][3]*mat1._m[2][2]*mat1._m[3][0] - mat1._m[1][2]*mat1._m[2][3]*mat1._m[3][0] - mat1._m[1][3]*mat1._m[2][0]*mat1._m[3][2] + mat1._m[1][0]*mat1._m[2][3]*mat1._m[3][2] + mat1._m[1][2]*mat1._m[2][0]*mat1._m[3][3] - mat1._m[1][0]*mat1._m[2][2]*mat1._m[3][3];
   inverse._m[1][1] = mat1._m[0][2]*mat1._m[2][3]*mat1._m[3][0] - mat1._m[0][3]*mat1._m[2][2]*mat1._m[3][0] + mat1._m[0][3]*mat1._m[2][0]*mat1._m[3][2] - mat1._m[0][0]*mat1._m[2][3]*mat1._m[3][2] - mat1._m[0][2]*mat1._m[2][0]*mat1._m[3][3] + mat1._m[0][0]*mat1._m[2][2]*mat1._m[3][3];
   inverse._m[1][2] = mat1._m[0][3]*mat1._m[1][2]*mat1._m[3][0] - mat1._m[0][2]*mat1._m[1][3]*mat1._m[3][0] - mat1._m[0][3]*mat1._m[1][0]*mat1._m[3][2] + mat1._m[0][0]*mat1._m[1][3]*mat1._m[3][2] + mat1._m[0][2]*mat1._m[1][0]*mat1._m[3][3] - mat1._m[0][0]*mat1._m[1][2]*mat1._m[3][3];
   inverse._m[1][3] = mat1._m[0][2]*mat1._m[1][3]*mat1._m[2][0] - mat1._m[0][3]*mat1._m[1][2]*mat1._m[2][0] + mat1._m[0][3]*mat1._m[1][0]*mat1._m[2][2] - mat1._m[0][0]*mat1._m[1][3]*mat1._m[2][2] - mat1._m[0][2]*mat1._m[1][0]*mat1._m[2][3] + mat1._m[0][0]*mat1._m[1][2]*mat1._m[2][3];
   inverse._m[2][0] = mat1._m[1][1]*mat1._m[2][3]*mat1._m[3][0] - mat1._m[1][3]*mat1._m[2][1]*mat1._m[3][0] + mat1._m[1][3]*mat1._m[2][0]*mat1._m[3][1] - mat1._m[1][0]*mat1._m[2][3]*mat1._m[3][1] - mat1._m[1][1]*mat1._m[2][0]*mat1._m[3][3] + mat1._m[1][0]*mat1._m[2][1]*mat1._m[3][3];
   inverse._m[2][1] = mat1._m[0][3]*mat1._m[2][1]*mat1._m[3][0] - mat1._m[0][1]*mat1._m[2][3]*mat1._m[3][0] - mat1._m[0][3]*mat1._m[2][0]*mat1._m[3][1] + mat1._m[0][0]*mat1._m[2][3]*mat1._m[3][1] + mat1._m[0][1]*mat1._m[2][0]*mat1._m[3][3] - mat1._m[0][0]*mat1._m[2][1]*mat1._m[3][3];
   inverse._m[2][2] = mat1._m[0][1]*mat1._m[1][3]*mat1._m[3][0] - mat1._m[0][3]*mat1._m[1][1]*mat1._m[3][0] + mat1._m[0][3]*mat1._m[1][0]*mat1._m[3][1] - mat1._m[0][0]*mat1._m[1][3]*mat1._m[3][1] - mat1._m[0][1]*mat1._m[1][0]*mat1._m[3][3] + mat1._m[0][0]*mat1._m[1][1]*mat1._m[3][3];
   inverse._m[2][3] = mat1._m[0][3]*mat1._m[1][1]*mat1._m[2][0] - mat1._m[0][1]*mat1._m[1][3]*mat1._m[2][0] - mat1._m[0][3]*mat1._m[1][0]*mat1._m[2][1] + mat1._m[0][0]*mat1._m[1][3]*mat1._m[2][1] + mat1._m[0][1]*mat1._m[1][0]*mat1._m[2][3] - mat1._m[0][0]*mat1._m[1][1]*mat1._m[2][3];
   inverse._m[3][0] = mat1._m[1][2]*mat1._m[2][1]*mat1._m[3][0] - mat1._m[1][1]*mat1._m[2][2]*mat1._m[3][0] - mat1._m[1][2]*mat1._m[2][0]*mat1._m[3][1] + mat1._m[1][0]*mat1._m[2][2]*mat1._m[3][1] + mat1._m[1][1]*mat1._m[2][0]*mat1._m[3][2] - mat1._m[1][0]*mat1._m[2][1]*mat1._m[3][2];
   inverse._m[3][1] = mat1._m[0][1]*mat1._m[2][2]*mat1._m[3][0] - mat1._m[0][2]*mat1._m[2][1]*mat1._m[3][0] + mat1._m[0][2]*mat1._m[2][0]*mat1._m[3][1] - mat1._m[0][0]*mat1._m[2][2]*mat1._m[3][1] - mat1._m[0][1]*mat1._m[2][0]*mat1._m[3][2] + mat1._m[0][0]*mat1._m[2][1]*mat1._m[3][2];
   inverse._m[3][2] = mat1._m[0][2]*mat1._m[1][1]*mat1._m[3][0] - mat1._m[0][1]*mat1._m[1][2]*mat1._m[3][0] - mat1._m[0][2]*mat1._m[1][0]*mat1._m[3][1] + mat1._m[0][0]*mat1._m[1][2]*mat1._m[3][1] + mat1._m[0][1]*mat1._m[1][0]*mat1._m[3][2] - mat1._m[0][0]*mat1._m[1][1]*mat1._m[3][2];
   inverse._m[3][3] = mat1._m[0][1]*mat1._m[1][2]*mat1._m[2][0] - mat1._m[0][2]*mat1._m[1][1]*mat1._m[2][0] + mat1._m[0][2]*mat1._m[1][0]*mat1._m[2][1] - mat1._m[0][0]*mat1._m[1][2]*mat1._m[2][1] - mat1._m[0][1]*mat1._m[1][0]*mat1._m[2][2] + mat1._m[0][0]*mat1._m[1][1]*mat1._m[2][2];
      
   float determinant;
   determinant =
   mat1._m[0][3] * mat1._m[1][2] * mat1._m[2][1] * mat1._m[3][0]-mat1._m[0][2] * mat1._m[1][3] * mat1._m[2][1] * mat1._m[3][0]-mat1._m[0][3] * mat1._m[1][1] * mat1._m[2][2] * mat1._m[3][0]+mat1._m[0][1] * mat1._m[1][3] * mat1._m[2][2] * mat1._m[3][0]+
   mat1._m[0][2] * mat1._m[1][1] * mat1._m[2][3] * mat1._m[3][0]-mat1._m[0][1] * mat1._m[1][2] * mat1._m[2][3] * mat1._m[3][0]-mat1._m[0][3] * mat1._m[1][2] * mat1._m[2][0] * mat1._m[3][1]+mat1._m[0][2] * mat1._m[1][3] * mat1._m[2][0] * mat1._m[3][1]+
   mat1._m[0][3] * mat1._m[1][0] * mat1._m[2][2] * mat1._m[3][1]-mat1._m[0][0] * mat1._m[1][3] * mat1._m[2][2] * mat1._m[3][1]-mat1._m[0][2] * mat1._m[1][0] * mat1._m[2][3] * mat1._m[3][1]+mat1._m[0][0] * mat1._m[1][2] * mat1._m[2][3] * mat1._m[3][1]+
   mat1._m[0][3] * mat1._m[1][1] * mat1._m[2][0] * mat1._m[3][2]-mat1._m[0][1] * mat1._m[1][3] * mat1._m[2][0] * mat1._m[3][2]-mat1._m[0][3] * mat1._m[1][0] * mat1._m[2][1] * mat1._m[3][2]+mat1._m[0][0] * mat1._m[1][3] * mat1._m[2][1] * mat1._m[3][2]+
   mat1._m[0][1] * mat1._m[1][0] * mat1._m[2][3] * mat1._m[3][2]-mat1._m[0][0] * mat1._m[1][1] * mat1._m[2][3] * mat1._m[3][2]-mat1._m[0][2] * mat1._m[1][1] * mat1._m[2][0] * mat1._m[3][3]+mat1._m[0][1] * mat1._m[1][2] * mat1._m[2][0] * mat1._m[3][3]+
   mat1._m[0][2] * mat1._m[1][0] * mat1._m[2][1] * mat1._m[3][3]-mat1._m[0][0] * mat1._m[1][2] * mat1._m[2][1] * mat1._m[3][3]-mat1._m[0][1] * mat1._m[1][0] * mat1._m[2][2] * mat1._m[3][3]+mat1._m[0][0] * mat1._m[1][1] * mat1._m[2][2] * mat1._m[3][3];
   
   Matrix::Multiply(inverse, (1/determinant),result);
}
void Matrix::OutputToConsole() const				
//output to a console
{for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if ((i==0) &&(j==0))
			{
			std::cout<<"<"<<Matrix::_m[i][j]<<", ";
			}
			else if ((i==3) &&(j==3))
			{
				std::cout<<Matrix::_m[i][j]<<">"<<std::endl;
			}
			else
			{
			std::cout<<Matrix::_m[i][j]<<", ";
			}
		}
	}
	
}
}