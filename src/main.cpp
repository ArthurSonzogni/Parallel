#include "animation/animation.h"
#include <cmath>
#include "physic/Body.h"
#include "physic/collision.h"

#include <iostream>
using namespace std;

int main(int argc, const char *argv[])
{
	if (true)
	{
		Body a,b;
		a.addPoint(Vecteur(0.0,0.0));
		a.addPoint(Vecteur(1.0,0.0));
		a.addPoint(Vecteur(1.0,1.0));
		a.addPoint(Vecteur(0.0,1.0));

		b.addPoint(Vecteur(0.5,0.5));
		b.addPoint(Vecteur(1.5,0.0));
		b.addPoint(Vecteur(2.0,0.5));
		b.addPoint(Vecteur(1.5,1.0));

		Collision c=a.isColliding(b);
		cout<<c.direction.x<<","<<c.direction.y<<endl;
		cout<<"---"<<endl;
		Collision d=b.isColliding(a);
		cout<<d.direction.x<<","<<d.direction.y<<endl;
	}
	if (false)
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
	}
	return 0;
}
