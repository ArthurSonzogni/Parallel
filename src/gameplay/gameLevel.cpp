#include "gameLevel.h"
#include "../physic/BroadPhase.h"
#include "../physic/collision.h"
#include <iostream>

using namespace sf;
using namespace std;

GameLevel::GameLevel(RenderWindow& s):
	screen(s)
{
	character.addPoint(Vecteur(-16.0,-64.0));
	character.addPoint(Vecteur(+16.0,-64.0));
	character.addPoint(Vecteur(+32.0,-32.0));
	character.addPoint(Vecteur(+32.0,+32.0));
	character.addPoint(Vecteur(+16.0,+64.0));
	character.addPoint(Vecteur(-16.0,+64.0));
	character.addPoint(Vecteur(-32.0,+32.0));
	character.addPoint(Vecteur(-32.0,-32.0));
	character.setMasse(0.25);
	character.setInertia(0.0);
	character.setPosition(Vecteur(50.0,0.0));

	for(int ax=0;ax<5;++ax)
	{
		for(int ay=0;ay<5;++ay)
		{
			if ( (ax+ay*13) % 2 ==0)
			{
				collisionBody.push_back(character);
				collisionBody.back().setPosition(Vecteur(100+ax*200,100+ay*200));
				collisionBody.back().setMasse(0.50);
				collisionBody.back().setInertia(1000.0);

			}
		}
	}

	
	// ajout d'un sol
	collisionBody.push_back(Body());
	collisionBody.back().addPoint(Vecteur(0.0,600.0));
	collisionBody.back().addPoint(Vecteur(0.0,550.0));
	collisionBody.back().addPoint(Vecteur(800.0,550.0));
	collisionBody.back().addPoint(Vecteur(800.0,600.0));
	collisionBody.back().setMasse(0.0);
	collisionBody.back().setInertia(0.0);
	
	allBodyRef.push_back(&character);
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
		// physic
		for(auto &it : allBodyRef)
		{
			if (not it->isLinearStatic())
				it->addImpulse(Vecteur(0.0,3.0));
		}

		const float detail=2;
		for(int i=0;i<detail;++i)
		{
			for(auto &it : allBodyRef)
				it->applyTime(1.0/60.0/detail);
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
		
		// graphic
		screen.clear(Color(0,0,0));
		character.draw(screen);
		for(auto &it : collisionBody)
			it.draw(screen);
		screen.display();	

		// time
		tt+=1.0/60.0-c.getElapsedTime().asSeconds();
		tttt+=1.0/60.0;
		if (ttt++%100==0)
			cout<<int(tt/tttt*100.0)<<endl;
		sf::sleep(sf::seconds(1.0/60.0)-c.getElapsedTime());
		c.restart();
		
	}
}


