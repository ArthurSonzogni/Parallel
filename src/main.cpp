#include "animation/animation.h"
#include <cmath>

int main(int argc, const char *argv[])
{
	sf::RenderWindow screen;
	screen.create(sf::VideoMode(640,480),"Parallel");
	Animation anim;
	anim.loadFromFile("anim/example.anim");
	sf::Clock c;
	sf::Clock cc;
	c.restart();
	cc.restart();
	while(true)
	{
		float tt=c.getElapsedTime().asSeconds();
		int yy=(tt/6.0);
		float t=tt-6.0*yy;
		if (t>=3.0)
		{
			t=6.0-t;
		}

		screen.clear(sf::Color(0,0,0));
		anim.draw(100,100,t,screen);
		screen.display();
		
		sf::sleep(sf::seconds(1.0/30.0)-cc.getElapsedTime());
		cc.restart();

	}
	return 0;
}
