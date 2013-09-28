#include "vecteur.h"
#include <cmath>

Vecteur::Vecteur(double X, double Y) :
	x(X),
	y(Y)
{
}

double Vecteur::length()
{
	return sqrt(x*x+y*y);
}

Vecteur operator+(Vecteur p1, Vecteur p2)
{
	return Vecteur(p1.x+p2.x,p1.y+p2.y);
}
Vecteur operator-(Vecteur p1, Vecteur p2)
{
	return Vecteur(p1.x-p2.x,p1.y-p2.y);
}
double operator*(Vecteur p1, Vecteur p2)
{
	return p1.x*p2.x+p1.y*p2.y;
}
double operator^(Vecteur p1, Vecteur p2)
{
	return p1.x*p2.y-p1.y*p2.x;
}
Vecteur operator*(double a, Vecteur p)
{
	return Vecteur(a*p.x,a*p.y);
}
Vecteur normalize(Vecteur p)
{
	return (1.0/p.length())*p;
}
