#pragma once
namespace renderer
{
class Vector;	// forward declaration

class Matrix
{
public: // constructors
	Matrix(void); //default constructor
	Matrix(float, float, float, float,
		   float, float, float, float,
		   float, float, float, float,
		   float, float, float, float); //parameterised constructor
	Matrix(const Matrix&); //copy constructor

public: // destructor
	~Matrix(void);

public:
	Matrix& operator=(Matrix& mat); //assignment operator

public: // equality
	bool Equals(Matrix&) const;

public: // arithmetic
	static void Add		 ( const Matrix& , const Matrix& , Matrix& );
	static void Subtract ( const Matrix& , const Matrix& , Matrix& );
	static void Multiply ( const Matrix& , float ,	   Matrix& );
	static void Multiply ( const Matrix& , const Matrix& , Matrix& );
	static void Transform( const Matrix& , const Vector& , Vector& );
	static void Transpose( const Matrix& , Matrix& );
	static void Rotate	 ( Matrix& , float , float , float );
	static void Inverse  ( const Matrix& , Matrix& );

public: // output
	void OutputToConsole() const;

public: // fields
	float _m[4][4];
};
}