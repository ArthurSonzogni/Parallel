#ifndef COLLISION_C1Q82B7T
#define COLLISION_C1Q82B7T

#include "vecteur.h"
class Collision
{
	public:
		Collision();
		bool isCollision;
		double penetration;
		Vecteur direction;
		Vecteur position;
};

#endif /* end of include guard: COLLISION_C1Q82B7T */
