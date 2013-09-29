#include "matrix.h"
#include <cmath>

Matrix::Matrix():
	v1(1.0,0.0),
	v2(0.0,1.0)
{
}

Matrix::Matrix(double angle):
	Matrix()
{
	setAngle(angle);
}

void Matrix::setAngle(double angle)
{
	float ca=cos(angle);
	float sa=sin(angle);
	v1=Vecteur(ca,-sa);
	v2=Vecteur(sa,ca);
}

Vecteur Matrix::operator*(Vecteur v)
{
	return Vecteur(v1*v,v2*v);
}
