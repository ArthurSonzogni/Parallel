#include "gameLevel.h"
#include "../physic/BroadPhase.h"
#include "../physic/collision.h"
#include <iostream>
#include <SFML/Window.hpp>

using namespace sf;
using namespace std;


GameLevel::GameLevel()
{
	map1 = NULL;
	map2 = NULL;
	screen = NULL;
}

GameLevel::~GameLevel()
{
	if (map1) delete map1;
	if (map2) delete map2;
}


void GameLevel::setScreen(sf::RenderWindow& s)
{
	screen=&s;
}

void GameLevel::setMap1(std::string m)
{
	if (map1==NULL) delete map1;
	map1 = new MapLoader(m);
}

void GameLevel::setMap2(std::string m)
{
	if (map2==NULL) delete map2;
	map2 = new MapLoader(m);
}

void GameLevel::execute()
{
	// ajout des objets fixes.
	collisionBody.insert(collisionBody.end(),map1->getFixedBody().begin(),map1->getFixedBody().end());

	character1.addPoint(Vecteur(-16.0,-64.0));
	character1.addPoint(Vecteur(+16.0,-64.0));
	character1.addPoint(Vecteur(+32.0,-48.0));
	character1.addPoint(Vecteur(+32.0,+48.0));
	character1.addPoint(Vecteur(+16.0,+64.0));
	character1.addPoint(Vecteur(-16.0,+64.0));
	character1.addPoint(Vecteur(-32.0,+48.0));
	character1.addPoint(Vecteur(-32.0,-48.0));
	character1.setMasse(0.25);
	character1.setInertia(0.0);
	character1.setPosition(Vecteur(400.0,0.0));

	character2 = character1;
	character2.setPosition(Vecteur(400,-200));

	Body triangle;
	for(float t=0;t<=6.28;t+=6.28/16.0)
	{
		triangle.addPoint(Vecteur(30.0*cos(t),30.0*sin(t)));
	}
	triangle.setMasse(0.1);
	triangle.setInertia(100.0);

	const float dx=150;
	for(int ax=0;ax<3;++ax)
	{
		for(int ay=0;ay<3;++ay)
		{
			if ( (ax+ay*13) % 2 ==0)
			{
				collisionBody.push_back(character1);
				collisionBody.back().setPosition(Vecteur(dx+ax*dx,dx+ay*dx));
				collisionBody.back().setMasse(0.50);
				collisionBody.back().setInertia(500.0);
			}
			else
			{
				collisionBody.push_back(triangle);
				collisionBody.back().setPosition(Vecteur(dx+ax*dx,dx+ay*dx));
				collisionBody.back().setMasse(0.50);
				collisionBody.back().setInertia(500.0);
			}
		}
	}
	

	
	
	allBodyRef.push_back(&character1);
	allBodyRef.push_back(&character2);
	for(auto &it : collisionBody)
	{
		allBodyRef.push_back(&it);
	}
	
	sf::Clock c;
	float tt=0.0;
	float tttt=0.0;
	int ttt=0;
	while(true)
	{
		// event
		Event event;
		while(screen->pollEvent(event));

		Vecteur v1=character1.getSpeed();
		Vecteur v2=character2.getSpeed();
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			if (v1.x<256.0)
				character1.addImpulse(Vecteur(30.0,0.0));
			if (v2.x<256.0)
				character2.addImpulse(Vecteur(30.0,0.0));
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			if (v1.x>-256.0)
				character1.addImpulse(Vecteur(-30.0,0.0));
			if (v2.x>-256.0)
				character2.addImpulse(Vecteur(-30.0,0.0));
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
				character1.addImpulse(Vecteur(0.0,-50.0));
		


		// physic
		for(auto &it : allBodyRef)
		{
			if (not it->isLinearStatic())
				it->addImpulse(Vecteur(0.0,10.0));
		}

		const float detail=2;
		for(int i=0;i<detail;++i)
		{
			for(auto &it : allBodyRef)
				it->applyTime(1.0/30.0/detail);
			BroadPhase bp(allBodyRef);
			auto &v = allBodyRef; // alias
			for(auto &a : bp.getOuput())
			{
				Collision ca=v[a.first]->isColliding(*v[a.second]);
				Collision cb=v[a.second]->isColliding(*v[a.first]);
				if (ca.isCollision)
					v[a.first]->addCollisionImpulse(*v[a.second],ca);
				if (cb.isCollision)
					v[a.second]->addCollisionImpulse(*v[a.first],cb);
			}
		}
		
		draw();

		// time
		tt+=1.0/30.0-c.getElapsedTime().asSeconds();
		tttt+=1.0/30.0;
		if (ttt++%100==0)
			cout<<int(tt/tttt*100.0)<<endl;
		sf::sleep(sf::seconds(1.0/30.0)-c.getElapsedTime());
		c.restart();
	}
}

void GameLevel::draw()
{
	screen->clear(Color(0,0,0));
	vector<TileMap>& background1 = map1->getTileMapBackground();
	for(auto &l : background1)
		screen->draw(l);
	for(auto &it : collisionBody)
		it.draw(*screen);
	character1.draw(*screen);
	character2.draw(*screen);
	vector<TileMap>& foreground1 = map1->getTileMapForeground();
	for(auto &l : foreground1)
		screen->draw(l);
	screen->display();
}

