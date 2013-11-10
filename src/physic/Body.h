#ifndef BODY_P1BCP0FY
#define BODY_P1BCP0FY

#include <vector>
#include "vecteur.h"
#include "matrix.h"
#include <SFML/Graphics.hpp>


class AABB;
class Collision;

#define GROUP_STATIC		0b1000000
#define GROUP_CHARACTER1	0b0100000
#define GROUP_CHARACTER2	0b0010000

#define GROUP_CHARACTER GROUP_CHARACTER1|GROUP_CHARACTER2
#define GROUP_ALL		GROUP_CHARACTER1|GROUP_CHARACTER2|GROUP_STATIC

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
		void recenter();
		
		void reoriente();
		void isCollisionPreviousStep();
		void setGroup(int g,int c);
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

		bool collisionPreviousStep;
};

#endif /* end of include guard: BODY_P1BCP0FY */
