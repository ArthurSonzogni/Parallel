#ifndef GAMELEVEL_ZBGFUVNO
#define GAMELEVEL_ZBGFUVNO

#include "../physic/Body.h"
#include <vector>
#include <SFML/Graphics.hpp>

class GameLevel
{
	public:
		GameLevel(sf::RenderWindow& s);
	private:
		sf::RenderWindow& screen;
		Body character;
		std::vector<Body> collisionBody;
		std::vector<Body*> allBodyRef;
};

#endif /* end of include guard: GAMELEVEL_ZBGFUVNO */
