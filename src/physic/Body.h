#ifndef BODY_P1BCP0FY
#define BODY_P1BCP0FY

#include <vector>
#include "vecteur.h"


class AABB;
class Collision;

// Body as a convexe polygon.
class Body
{
	public:
		Body();
		AABB getAABB();
		Collision isColliding(Body& other,bool testOther);

		void addPoint(Vecteur v);
	private:

		double invmass;
		std::vector<Vecteur>	pointList;
		Vecteur position;
		Vecteur speed;
};

#endif /* end of include guard: BODY_P1BCP0FY */
