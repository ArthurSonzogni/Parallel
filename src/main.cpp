#include "animation/animation.h"
#include <cmath>
#include "physic/Body.h"
#include "physic/collision.h"
#include "physic/BroadPhase.h"
#include <random>

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
		a.setMasse(0.25);
		a.setInertia(1000.0);


		b.addPoint(Vecteur(-48,+0.0));
		b.addPoint(Vecteur(+0.0,-32.0));
		b.addPoint(Vecteur(+48,+0.0));
		b.addPoint(Vecteur(+0.0,+32.0));
		b.setPosition(Vecteur(2.4*32,0.0));
		b.setMasse(0.25);
		b.setInertia(1000.0);

		vector<Body> generated;
		for(int ax=0;ax<5;++ax)
		{
			for(int ay=0;ay<5;++ay)
			{
				if ( (ax+ay*13) % 2 ==0)
					generated.push_back(a);
				else
					generated.push_back(b);
				generated.back().setPosition(Vecteur(10+ax*200,10+ay*200));
				generated.back().setMasse(0.25);
				generated.back().setInertia(1000.0);
			}
		}

		sf::RenderWindow screen;
		sf::ContextSettings settings;
		settings.antialiasingLevel = 4;
		screen.create(sf::VideoMode(800,600),"Parallel",sf::Style::Default,settings);
		
		sf::Clock c;
		float angle=0.0;

		vector<Body*> v;
		v.push_back(&a);
		v.push_back(&b);
		for(auto &i : generated)
		{
			v.push_back(&i);
		}
		
		default_random_engine generator;
		normal_distribution<double> distribution(0.0,100.0);

		for(auto &it : v)
		{
			double vx=distribution(generator);
			double vy=distribution(generator);
			it->setSpeed(Vecteur(vx,vy));
		}

		while(true)
		{
			screen.clear(sf::Color(0,0,0));	

			const float detail=2;
			for(int i=0;i<detail;i++)
			{
				for(auto &it : v)
					it->applyTime(1.0/60.0/detail);

				BroadPhase bp(v);
				for(auto &a : bp.getOuput())
				{
					Collision ca=v[a.first]->isColliding(*v[a.second]);
					Collision cb=v[a.second]->isColliding(*v[a.first]);
					if (ca.penetration>cb.penetration)
					{
						if (ca.isCollision)
							v[a.first]->addCollisionImpulse(*v[a.second],ca);

					}
					else
					{
						if (cb.isCollision)
							v[a.second]->addCollisionImpulse(*v[a.first],cb);
					}
				}
			}
			for(auto &it : v)
			{
				it->draw(screen);
			}
			sf::Vector2i mouse=sf::Mouse::getPosition(screen);
			auto mouse2=screen.mapPixelToCoords(sf::Vector2i(mouse.x,mouse.y));
			Vecteur mo=Vecteur(mouse2.x,mouse2.y);
			Vecteur mv=Vecteur(mouse2.y,mouse2.x);
			Vecteur pa=a.getPosition();
			Vecteur pb=b.getPosition();
			a.addImpulse(0.5*(mo-pa));
			//b.addImpulse(0.5*(mv-pb));
			
			a.setSpeed(0.92*a.getSpeed());
			//b.setSpeed(0.92*b.getSpeed());

			
			
			//cout<<"ratio="<<bp.getOuput().size()*200/v.size()/v.size()<<endl;

			screen.display();
			sf::sleep(sf::seconds(1.0/60.0)-c.getElapsedTime());
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
			
			sf::sleep(sf::seconds(1.0/60.0)-cc.getElapsedTime());
			cc.restart();

		}
	}
	return 0;
}
