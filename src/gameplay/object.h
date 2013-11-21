#ifndef OBJECT_FMN5CELU
#define OBJECT_FMN5CELU

#include <SFML/Graphics.hpp>
#include <vector>
#include "../physic/Body.h"

class Object
{
	public:
		virtual void draw(sf::RenderWindow& screen) = 0;
		virtual void step() = 0;
		virtual std::vector<Body*> getBodies() = 0;
};

#endif /* end of include guard: OBJECT_FMN5CELU */
