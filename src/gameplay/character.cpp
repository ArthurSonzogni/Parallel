#include "character.h"
#include <iostream>

using namespace std;
using namespace sf;

Character::Character()
{
	body.addPoint(Vecteur(-16.0,-64.0));
	body.addPoint(Vecteur(+16.0,-64.0));
	body.addPoint(Vecteur(+32.0,-48.0));
	body.addPoint(Vecteur(+32.0,+48.0));
	body.addPoint(Vecteur(+16.0,+64.0));
	body.addPoint(Vecteur(-16.0,+64.0));
	body.addPoint(Vecteur(-32.0,+48.0));
	body.addPoint(Vecteur(-32.0,-48.0));
	body.setMasse(0.50);
	body.setInertia(0.0);
	body.recenter();
	body.setGroup(GROUP_CHARACTER,GROUP_COLLISION_CHARACTER);
	body.setRestitution(0.0);
	body.setFriction(1.0);
	
	jumpDetector.addPoint(Vecteur(-16.0,-0.0));
	jumpDetector.addPoint(Vecteur(+16.0,-0.0));
	jumpDetector.addPoint(Vecteur(0.0,+1.0));
	jumpDetector.setMasse(0.0);
	jumpDetector.setInertia(0.0);
	jumpDetector.recenter();
	jumpDetector.setGroup(GROUP_JUMPDETECTOR,GROUP_COLLISION_JUMPDETECTOR);
}

void Character::draw(sf::RenderWindow& screen)
{
	cout<<(jumpDetector.isCollisionPreviousStep())<<endl;
	body.draw(screen);
}

vector<Body*> Character::getAllBodyRef()
{
	vector<Body*> v;
	v.push_back(&body);
	v.push_back(&jumpDetector);
	return v;
}

void Character::setPosition(Vecteur p)
{
	body.setPosition(p);
	jumpDetectorResetPosition();
}

Vecteur Character::getPosition()
{
	return body.getPosition();
}

void Character::keyLeft()
{
	if (body.getSpeed().x>-256.0)
	if (jumpDetector.isCollisionPreviousStep())
		body.addImpulse(Vecteur(-30.0,0.0));
}
void Character::keyRight()
{
	if (body.getSpeed().x<256.0)
	if (jumpDetector.isCollisionPreviousStep())
		body.addImpulse(Vecteur(30.0,0.0));
}
void Character::keyUp()
{
	if (jumpDetector.isCollisionPreviousStep())
		body.addImpulse(Vecteur(0.0,-120.0));
}

void Character::jumpDetectorResetPosition()
{
	jumpDetector.setPosition(body.getPosition()+Vecteur(0.0,64.0+2.0));
}

void Character::step()
{
	jumpDetectorResetPosition();
}
