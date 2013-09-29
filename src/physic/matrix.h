#ifndef MATRIX_LIFZAQEY
#define MATRIX_LIFZAQEY

#include "vecteur.h"

class Matrix
{
	public:
		Matrix();
		Matrix(double angle);
		void setAngle(double angle);
		Vecteur v1,v2;
		Vecteur operator*(Vecteur v);
};

#endif /* end of include guard: MATRIX_LIFZAQEY */
