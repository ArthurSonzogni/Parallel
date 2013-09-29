#include "animation/animation.h"
#include <cmath>
#include "physic/Body.h"
#include "physic/collision.h"
#include "physic/BroadPhase.h"

#include <iostream>
using namespace std;

void print(set<pair<int,int>> i)
{
	for(auto &a : i)
	{
		cout<<"("<<a.first<<","<<a.second<<")";
	}
	cout<<endl;
}

int main(int argc, const char *argv[])
{
	{
		Body a,b;
		a.addPoint(Vecteur(-32.0,-32.0));
		a.addPoint(Vecteur(+32.0,-32.0));
		a.addPoint(Vecteur(+32.0,+32.0));
		a.addPoint(Vecteur(-32.0,+32.0));
		a.setPosition(Vecteur(0.0,0.0));


		b.addPoint(Vecteur(-48,+0.0));
		b.addPoint(Vecteur(+0.0,-32.0));
		b.addPoint(Vecteur(+48,+0.0));
		b.addPoint(Vecteur(+0.0,+32.0));
		b.setPosition(Vecteur(2.4*32,0.0));

		Body b1=b;
		Body b2=b;
		Body b3=b;
		Body b4=b;

		sf::RenderWindow screen;
		sf::ContextSettings settings;
		settings.antialiasingLevel = 4;
		screen.create(sf::VideoMode(800,600),"Parallel",sf::Style::Default,settings);
		
		sf::Clock c;
		float angle=0.0;
		while(true)
		{
			screen.clear(sf::Color(0,0,0));	

			sf::Vector2i mouse=sf::Mouse::getPosition(screen);
			auto mouse2=screen.mapPixelToCoords(sf::Vector2i(mouse.x,mouse.y));
			a.setPosition(Vecteur(mouse2.y,mouse2.x));
			b.setPosition(Vecteur(mouse2.x,mouse2.y));
			b1.setPosition(Vecteur(100,100));
			b2.setPosition(Vecteur(400,100));
			b3.setPosition(Vecteur(400,400));
			b4.setPosition(Vecteur(100,400));
			a.setAngle(angle);
			b.setAngle(angle);
			angle+=6.12/30.0*0.1;
			
			a.draw(screen);
			b.draw(screen);
			b1.draw(screen);
			b2.draw(screen);
			b3.draw(screen);
			b4.draw(screen);
			
			vector<Body*> v;
			v.push_back(&a);
			v.push_back(&b);
			v.push_back(&b1);
			v.push_back(&b2);
			v.push_back(&b3);
			v.push_back(&b4);
			BroadPhase bp(v,screen);
			print(bp.getOuput());

			screen.display();
			sf::sleep(sf::seconds(1.0/30.0)-c.getElapsedTime());
			c.restart();
		}

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
