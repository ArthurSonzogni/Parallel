#ifndef GAMELEVEL_ZBGFUVNO
#define GAMELEVEL_ZBGFUVNO

#include "../physic/Body.h"
#include "../map/mapLoader.h"
#include <vector>
#include <SFML/Graphics.hpp>

class GameLevel
{
	public:
		GameLevel();
		~GameLevel();
		void setScreen(sf::RenderWindow& s);
		void setMap1(std::string map1);
		void setMap2(std::string map2);
		void execute();
	private:
		MapLoader* map1;
		MapLoader* map2;
		sf::RenderWindow* screen;
		Body character1,character2;
		std::vector<Body> collisionBody;
		std::vector<Body*> allBodyRef;
		void draw();
};

#endif /* end of include guard: GAMELEVEL_ZBGFUVNO */
