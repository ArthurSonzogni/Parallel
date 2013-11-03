#ifndef BODY_P1BCP0FY
#define BODY_P1BCP0FY

#include <vector>
#include "vecteur.h"
#include "matrix.h"
#include <SFML/Graphics.hpp>


class AABB;
class Collision;

// Body as a convexe polygon.
class Body
{
	public:
		Body();
		AABB getAABB();
		std::vector<Collision> isColliding(Body& other);

		void addPoint(Vecteur v);
		void setPosition(Vecteur p);
		Vecteur getPosition();
		void addCollisionImpulse(Body& other,Collision& c);
		void applyTime(double t);
		void draw(sf::RenderWindow& screen);
		void setAngle(double angle);
		void setSpeed(Vecteur speed);
		Vecteur getSpeed();
		void addImpulse(Vecteur impulse);
		void addTorque(double torque);
		void setMasse(double mass);
		void setInertia(double inertia);
		void setRestitution(double r);
		void setFriction(double f);
		bool isLinearStatic();
		
		void reoriente();
	private:
		
		int group;
		int collisionGroup;
		float restitution;
		float friction;
		std::vector<Vecteur>	pointList;

		Vecteur position;
		Vecteur speed;
		double invMass;

		Matrix	orientation;
		double  angle;
		double  angularSpeed;
		double  invInertia;
		void updateOrientation();


};

#endif /* end of include guard: BODY_P1BCP0FY */
