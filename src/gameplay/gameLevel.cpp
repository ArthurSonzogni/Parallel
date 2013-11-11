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
	resetViews();
}
void GameLevel::resetViews()
{
	view1.setSize(Vector2f(screen->getSize().x,screen->getSize().y*0.5));			
	view2.setSize(Vector2f(screen->getSize().x,screen->getSize().y*0.5));			
	view1.setViewport(FloatRect(0,0,1,0.5));
	view2.setViewport(FloatRect(0,0.5,1,0.5));
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

		
	
	{
		auto v1 = character1.getAllBodyRef();
		auto v2 = character2.getAllBodyRef();
		allBodyRef1.insert(allBodyRef1.end(),v1.begin(),v1.end());
		allBodyRef2.insert(allBodyRef2.end(),v2.begin(),v2.end());
	}

	character1.setPosition(Vecteur(200,0));
	character2.setPosition(Vecteur(200,0));

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
		while(screen->pollEvent(event))
		{
			if (event.type == sf::Event::Resized)			
				resetViews();
		}

		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			character1.keyLeft();
			character2.keyLeft();
		}

		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			character1.keyRight();
			character2.keyRight();
		}

		if (Keyboard::isKeyPressed(Keyboard::Up))
		{
			character1.keyUp();
			character2.keyUp();
		}
		
		character1.step();
		character2.step();

		// physic
		for(auto &it : allBodyRef1)
		{
			if (not it->isLinearStatic())
				it->addImpulse(Vecteur(0.0,20.0));
		}
		for(auto &it : allBodyRef2)
		{
			if (not it->isLinearStatic())
				it->addImpulse(Vecteur(0.0,20.0));
		}

		draw();
		const float detail=1;
		for(int i=0;i<detail;++i)
		{
			for(auto &it : allBodyRef1)
				it->applyTime(1.0/30.0/detail);
			for(auto &it : allBodyRef2)
				it->applyTime(1.0/30.0/detail);

			BroadPhase bp1(allBodyRef1);
			BroadPhase bp2(allBodyRef2);

			cout<<"bp1.getOutput()="<<bp1.getOutput().size()<<endl;
			cout<<"bp2.getOutput()="<<bp2.getOutput().size()<<endl;

			for(auto &a : bp1.getOutput())
			{
				auto &v = allBodyRef1; // alias
				vector<Collision> ca=v[a.first]->isColliding(*v[a.second]);
				vector<Collision> cb=v[a.second]->isColliding(*v[a.first]);

				for(auto &collision : ca)
				{
					v[a.first]->addCollisionImpulse(*v[a.second],collision);
				}
				for(auto &collision : cb)
				{
					v[a.second]->addCollisionImpulse(*v[a.first],collision);
				}
			}
			for(auto &a : bp2.getOutput())
			{
				auto &v = allBodyRef2; // alias
				vector<Collision> ca=v[a.first]->isColliding(*v[a.second]);
				vector<Collision> cb=v[a.second]->isColliding(*v[a.first]);
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
	updateViewCenter();

	screen->clear(Color(0,0,0));
	// vue 1
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


void viewdiff(float& current, float &target)
{
	float d=target-current;
	if (d<-1.0 or d>1.0)
		current+=d*0.1;
}
void GameLevel::updateViewCenter()
{
	Vecteur v1=character1.getPosition();
	Vecteur v2=character2.getPosition();
	float x1=v1.x;
	float y1=v1.y;
	float x2=v2.x;
	float y2=v2.y;
	viewdiff(view1x,x1);
	viewdiff(view1y,y1);
	viewdiff(view2x,x2);
	viewdiff(view2y,y2);
	view1.setCenter(Vector2f(int(view1x),int(view1y)));
	view2.setCenter(Vector2f(int(view2x),int(view2y)));
}
