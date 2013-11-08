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
		sf::View view1;
		sf::View view2;
		Body character1;
		Body character2;
		sf::RenderWindow* screen;
		std::vector<Body> collisionBody1;
		std::vector<Body> collisionBody2;
		std::vector<Body*> allBodyRef1;
		std::vector<Body*> allBodyRef2;
		void draw();
};

#endif /* end of include guard: GAMELEVEL_ZBGFUVNO */
