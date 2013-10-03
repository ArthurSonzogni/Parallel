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
	invMass = 4.0;
	other.invMass = 4.0;
	invInertia = 0.001;
	other.invInertia = 0.001;
	// the two objects aren't movable
	if (invMass+other.invMass == 0.0) return; 
	
	Vecteur r0= c.position - position;
	Vecteur r1= c.position - other.position;
	Vecteur v0= speed  + (angularSpeed^r0);
	Vecteur v1= other.speed  + (other.angularSpeed^r1);
	// relative velocity
	Vecteur dv=v0-v1;
	
	Vecteur normal=(-1.0)*c.direction;
	
	// if the two object are moving away from each other
	double relativeMovement=-(dv*normal);
	if (relativeMovement<-0.01)
		return;
	
	double jn,jt;
	// Normal Impulse
	{
		// coefficient of restitution
		double e=0.01;
		double normDiv = (normal*normal)*(invMass + other.invMass) 
						+ normal*
						 (
						  	((invInertia*(r0^normal))^r0) +
						  	((other.invInertia*(r1^normal))^r1) 
						 )
						;
		
		jn =  (-1.0 - e) * (dv*normal) / normDiv;

		// bias impulse to force object to don't penetrate each other.
		jn+=(c.penetration*1.5);
		
		addImpulse( (invMass*jn) * normal);		
		other.addImpulse((-other.invMass*jn)*normal);
		
		addTorque(invInertia*jn*(r0^normal));
		other.addTorque((-other.invInertia*jn)*(r1^normal));
		
						   
	}
	
	//Tangent Impulse
	{
		Vecteur tangent= dv - (dv*normal)*normal;
		tangent=normalize(tangent);
		double tangDiv = invMass + other.invMass;
			+ (tangent *
			(
				 (invInertia*(r0^tangent)^r0)+
				 (other.invInertia*(r1^tangent)^r1)
			)
			);
		jt = (-1.0)* (dv*tangent)/tangDiv;
		
		const float coef=0.1;
		if (jt>coef*jn)
			jt=coef*jn;
		
		
		addImpulse(invMass * jt  * tangent);
		other.addImpulse(-other.invMass * jt * tangent);
		
		addTorque(invInertia * (r0^(jt*tangent)));
		other.addTorque(other.invInertia * (r1^(jt*tangent)));

		

	}

}

void Body::applyTime(double t)
{
	position = position + t*speed;
	angle = angle + t*angularSpeed;
	updateOrientation();
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

void Body::setAngle(double Angle)
{
	angle=Angle;
	updateOrientation();
}

void Body::setSpeed(Vecteur Speed)
{
	speed=Speed;
}
void Body::addImpulse(Vecteur impulse)
{
	speed = speed + impulse;
}
void Body::addTorque(double torque)
{
	angularSpeed = angularSpeed + torque;
}
Vecteur Body::getPosition()
{
	return position;
}
Vecteur Body::getSpeed()
{
	return speed;
}
