#include "gameLevel.h"
#include "../physic/BroadPhase.h"
#include "../physic/collision.h"
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

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
	view1.setCenter(Vector2f(s.getSize().x*0.5,s.getSize().y*0.25));
	view2.setCenter(Vector2f(s.getSize().x*0.5,s.getSize().y*0.25));
	view1.setSize(Vector2f(s.getSize().x,s.getSize().y*0.5));			
	view2.setSize(Vector2f(s.getSize().x,s.getSize().y*0.5));			
	view1.setViewport(FloatRect(0,0,1,0.5));
	view2.setViewport(FloatRect(0,0.5,1,1));
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
	collisionBody1.insert(collisionBody1.end(),map1->getFixedBody().begin(),map1->getFixedBody().end());
	collisionBody2.insert(collisionBody2.end(),map2->getFixedBody().begin(),map2->getFixedBody().end());

	character1.addPoint(Vecteur(-16.0,-64.0));
	character1.addPoint(Vecteur(+16.0,-64.0));
	character1.addPoint(Vecteur(+32.0,-48.0));
	character1.addPoint(Vecteur(+32.0,+48.0));
	character1.addPoint(Vecteur(+16.0,+64.0));
	character1.addPoint(Vecteur(-16.0,+64.0));
	character1.addPoint(Vecteur(-32.0,+48.0));
	character1.addPoint(Vecteur(-32.0,-48.0));
	character1.setMasse(0.50);
	character1.setInertia(0.0);
	character1.setPosition(Vecteur(400.0,0.0));

	character2 = character1;
	character2.setPosition(Vecteur(400,-200));

	Body triangle;
	for(float t=0;t<=6.28;t+=6.28/16.0)
	{
		triangle.addPoint(Vecteur(30.0*cos(t),30.0*sin(t)));
	}
	triangle.setMasse(0.50);
	triangle.setInertia(100.0);

	const float dx=150;
	for(int ax=0;ax<3;++ax)
	{
		for(int ay=0;ay<3;++ay)
		{
			if ( (ax+ay*13) % 5 ==0)
			{
				collisionBody1.push_back(character1);
				collisionBody1.back().setPosition(Vecteur(dx+ax*dx,dx+ay*dx));
				collisionBody1.back().setMasse(0.50);
				collisionBody1.back().setInertia(500.0);
			}
			else
			{
				collisionBody2.push_back(triangle);
				collisionBody2.back().setPosition(Vecteur(dx+ax*dx,dx+ay*dx));
				collisionBody2.back().setMasse(0.50);
				collisionBody2.back().setInertia(500.0);
			}
		}
	}
	
	
	allBodyRef1.push_back(&character1);
	allBodyRef2.push_back(&character2);
	for(auto &it : collisionBody1)
	{
		allBodyRef1.push_back(&it);
	}
	for(auto &it : collisionBody2)
	{
		allBodyRef2.push_back(&it);
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
		for(auto &it : allBodyRef1)
		{
			if (not it->isLinearStatic())
				it->addImpulse(Vecteur(0.0,10.0));
		}
		for(auto &it : allBodyRef2)
		{
			if (not it->isLinearStatic())
				it->addImpulse(Vecteur(0.0,10.0));
		}

		draw();
		const float detail=2;
		for(int i=0;i<detail;++i)
		{
			for(auto &it : allBodyRef1)
				it->applyTime(1.0/30.0/detail);
			for(auto &it : allBodyRef2)
				it->applyTime(1.0/30.0/detail);

			BroadPhase bp1(allBodyRef1);
			BroadPhase bp2(allBodyRef2);
			for(auto &a : bp1.getOuput())
			{
				auto &v = allBodyRef1; // alias
				vector<Collision> ca=v[a.first]->isColliding(*v[a.second]);
				vector<Collision> cb=v[a.second]->isColliding(*v[a.first]);
				Collision cca=add(ca);
				Collision ccb=add(cb);
				for(auto &collision : ca)
				{
					v[a.first]->addCollisionImpulse(*v[a.second],collision);
				}
				for(auto &collision : cb)
				{
					v[a.second]->addCollisionImpulse(*v[a.first],collision);
				}
			}
			for(auto &a : bp2.getOuput())
			{
				auto &v = allBodyRef2; // alias
				vector<Collision> ca=v[a.first]->isColliding(*v[a.second]);
				vector<Collision> cb=v[a.second]->isColliding(*v[a.first]);
				Collision cca=add(ca);
				Collision ccb=add(cb);
				for(auto &collision : ca)
				{
					v[a.first]->addCollisionImpulse(*v[a.second],collision);
				}
				for(auto &collision : cb)
				{
					v[a.second]->addCollisionImpulse(*v[a.first],collision);
				}
			}
		}
		
		screen->display();

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

	static int j=0;
	static int i=0;
	if (j++%20==0)
	i++;
	// vue 1
	//if(i%2==0)
	{
		screen->setView(view1);
		vector<TileMap>& background = map1->getTileMapBackground();
		for(auto &l : background)
			screen->draw(l);
		for(auto &it : collisionBody1)
			it.draw(*screen);
		character1.draw(*screen);
		vector<TileMap>& foreground = map1->getTileMapForeground();
		for(auto &l : foreground)
			screen->draw(l);
	}
	// vue 2
	//if(i%2==1)
	{
		screen->setView(view2);
		vector<TileMap>& background = map2->getTileMapBackground();
		for(auto &l : background)
			screen->draw(l);
		for(auto &it : collisionBody2)
			it.draw(*screen);
		character2.draw(*screen);
		vector<TileMap>& foreground = map2->getTileMapForeground();
		for(auto &l : foreground)
			screen->draw(l);
	}
}

