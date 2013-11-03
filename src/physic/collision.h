#ifndef COLLISION_C1Q82B7T
#define COLLISION_C1Q82B7T

#include "vecteur.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class Collision
{
	public:
		Collision();
		double penetration;
		Vecteur direction;
		Vecteur position;
		void draw(sf::RenderWindow& screen);
};
Collision add(std::vector<Collision>& v);

#endif /* end of include guard: COLLISION_C1Q82B7T */
