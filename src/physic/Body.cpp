#include "Body.h"
#include "AABB.h"
#include "collision.h"

#include <iostream>
using namespace std;


Body::Body():
	position(0.0,0.0),
	speed(0.0,0.0),
	invMass(0.0),
	angle(0.0),
	angularSpeed(0.0),
	invInertia(0.0)
{
	
}

double min(double a, double b)
{
	return (a<b) ? a : b;
}

double max(double a, double b)
{
	return (a>b) ? a : b;
}

AABB Body::getAABB()
{
	auto p=pointList.begin();
	Vecteur v=orientation*(*p)+position;
	AABB box(v.x,v.x,v.y,v.y);
	++p;
	while(p!=pointList.end())
	{
		Vecteur v=orientation*(*p)+position;
		box.minx=min(box.minx,v.x);
		box.maxx=max(box.maxx,v.x);
		box.miny=min(box.miny,v.y);
		box.maxy=max(box.maxy,v.y);
		++p;
	}
	return box;
}

Collision Body::isColliding(Body& other)
{
	int nbEdge=pointList.size();
	int nbEdgeOther=other.pointList.size();

	double bestPenetration=0.0;
	Vecteur bestNormale(0.0,0.0);
	Vecteur bestPosition(0.0,0.0);

	double penetration;

	for(int i=0;i<nbEdge;++i)
	{
		double subBestPenetration=100000.0;
		Vecteur subBestNormale(0.0,0.0);
		bool in=true;
		Vecteur myPoint=orientation*pointList[i]+position;
		for(int j=0;j<nbEdgeOther;++j)
		{
			int next=(j+1)%nbEdgeOther;
			Vecteur v=normalize(other.orientation*(other.pointList[next]-other.pointList[j]));
			penetration=v^(myPoint-other.orientation*(other.pointList[j])-other.position);
			if (penetration<0.0)
			{
				in=false;
				break;
			}
			else if (penetration<subBestPenetration)
			{
				subBestPenetration=penetration;
				subBestNormale=v.getNormale();
			}
		}
		if (in)
		{
			if (subBestPenetration>bestPenetration)
			{
				bestPenetration=subBestPenetration;
				bestNormale=subBestNormale;
				bestPosition=myPoint;
			}
		}
	}
	if (bestPenetration>0.0)
	{
		Collision c;
		c.isCollision=true;
		c.penetration=bestPenetration;
		c.direction=normalize(bestNormale);
		c.position=bestPosition;
		return c;
	}
	else
	{
		return Collision();	
	}
}

void Body::addPoint(Vecteur v)
{
	pointList.push_back(v);
}

void Body::setPosition(Vecteur p)
{
	position=p;
}

void Body::addCollisionImpulse(Body& other,Collision& c)
{
	// the two objects aren't movable
	if (invMass+other.invMass == 0.0) return; 
	
	Vecteur r0= position - position;
	Vecteur r1= other.position - other.position;
	Vecteur v0= speed  - (angularSpeed^r0);
	Vecteur v1= other.speed  - (other.angularSpeed^r1);
	// relative velocity
	Vecteur dv=v0-v1;
	

	// if the two object are moving away from each other
	float relativeMovement=(dv*c.direction);
	if (relativeMovement<0.01)
		return;
	
	// Normal Impulse
	{
		// coefficient of restitution
		float e=0.5;
		e=e;
		
	}
	
	//Tangent Impulse
	{

	}

}

void Body::applyTime(double t)
{
	position = position + t*speed;
	angle = angle + t*angularSpeed;
}

void Body::updateOrientation()
{
	orientation.setAngle(angle);
}

void Body::draw(sf::RenderWindow& screen)
{
	sf::ConvexShape s;
	int i=0;
	s.setPointCount(pointList.size());
	for (auto &p : pointList)
	{
		s.setPoint(i,sf::Vector2f(p.x,p.y));
		++i;
	}
	#define RADTODEG 57.2957795131
	s.rotate(angle*RADTODEG);
	s.move(sf::Vector2f(position.x,position.y));
	screen.draw(s);
}

void Body::setAngle(float Angle)
{
	angle=Angle;
	updateOrientation();
}
