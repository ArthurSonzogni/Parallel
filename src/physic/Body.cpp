#include "Body.h"
#include "AABB.h"
#include "collision.h"
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;


#define minWeight 0.0001

bool equalZero(double d)
{
	return d*d<0.0001;
}
bool equalZero(Vecteur d)
{
	return d*d<0.0001;
}

Body::Body():
	group(0),
	collisionGroup(0),
	restitution(0.5),
	friction(0.1),
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

vector<Collision> Body::isColliding(Body& other)
{
	int nbEdge=pointList.size();
	int nbEdgeOther=other.pointList.size();
	double penetration;
	vector<Collision> retour;

	for(int i=0;i<nbEdge;++i)
	{
		double bestPenetration=1000000.0;
		Vecteur bestNormale(0.0,0.0);
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
			else if (penetration<bestPenetration)
			{
				bestPenetration=penetration;
				bestNormale=v.getNormale();
			}
		}
		if (in)
		{
			Collision c;
			c.penetration=bestPenetration;
			c.direction=normalize(bestNormale);
			c.position=myPoint;
			retour.push_back(c);
		}
	}
	return retour;
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
	if (invMass+other.invMass < minWeight) return; 
	
	
	Vecteur r0= c.position - position;
	Vecteur r1= c.position - other.position;
	Vecteur v0= speed  + (angularSpeed^r0);
	Vecteur v1= other.speed  + (other.angularSpeed^r1);
	// relative velocity
	Vecteur dv=v0-v1;
	
	Vecteur normal=(-1.0)*c.direction;
	
	// if the two object are moving away from each other
	double relativeMovement=-(dv*normal);
	if (relativeMovement<-0.0001)
		return;
	
	double jn,jt;
	// Normal Impulse
	{
		// coefficient of restitution
		double e=(restitution+other.restitution)*0.5;
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
		if (tangent*tangent<minWeight) return;
		tangent=normalize(tangent);
		double tangDiv = invMass + other.invMass
			+ (tangent *
				(
					 (invInertia*(r0^tangent)^r0)+
					 (other.invInertia*(r1^tangent)^r1)
				)
			);
		jt = (-1.0)* (dv*tangent)/tangDiv;
		
		float coef=(friction+other.friction)*0.5;
		if (jt>coef*jn)
			jt=coef*jn;
		else if (jt<-coef*jn)
			jt=-coef*jn;
		
		
		addImpulse(invMass * jt  * tangent);
		other.addImpulse(-other.invMass * jt * tangent);
		
		addTorque(invInertia * (r0^(jt*tangent)));
		other.addTorque(other.invInertia * (r1^(jt*tangent)));

	}

}

void Body::applyTime(double t)
{
	speed=0.9999*speed;
	angularSpeed*=0.999999;

	if (equalZero(speed)) speed=Vecteur(0.0,0.0);
	else position = position + t*speed;

	if (equalZero(angularSpeed)) angularSpeed=0.0;
	else angle = angle + t*angularSpeed;

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
	s.setFillColor(sf::Color(255,255,255,128));
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
void Body::setMasse(double mass)
{
	if (mass<minWeight)
		invMass=0.0;
	else
		invMass=1.0/mass;
}
void Body::setInertia(double inertia)
{
	if (inertia<minWeight)
		invInertia=0.0;
	else
		invInertia=1.0/inertia;
}

bool Body::isLinearStatic()
{
	return invMass==0.0;
}
void Body::setRestitution(double r)
{
	restitution=r;
}
void Body::setFriction(double f)
{
	friction=f;
}
void Body::reoriente()
{
	if (pointList.size()<=2) return;
	Vecteur ab=pointList[1]-pointList[0];
	Vecteur bc=pointList[2]-pointList[1];
	float r=bc^ab;
	if (r>0.0)
	{
		reverse(pointList.begin(),pointList.end());
		cout<<"je reverse"<<endl;
	}
}
