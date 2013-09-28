#include "Body.h"
#include "AABB.h"
#include "collision.h"

Body::Body():
	position(0.0,0.0),
	speed(0.0,0.0)
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
	AABB box(p->x,p->x,p->y,p->y);
	p++;
	for(;p!=pointList.begin();++p)
	{
		box.minx=min(box.minx,p->x);
		box.maxx=max(box.maxx,p->x);
		box.miny=min(box.miny,p->y);
		box.maxy=max(box.maxy,p->y);
	}
	return box;
}

#include <iostream>
using namespace std;
Collision Body::isColliding(Body& other, bool testOther)
{
	int nbEdge=pointList.size();
	int nbEdgeOther=other.pointList.size();

	double bestPenetration=0.0;
	Vecteur bestNormale(0.0,0.0);
	Vecteur bestPosition(0.0,0.0);

	double penetration;

	for(int i=0;i<nbEdge;++i)
	{
		double subBestPenetration=0.0;
		Vecteur subBestNormale(0.0,0.0);
		Vecteur subBestPosition(0.0,0.0);
		bool in=true;
		for(int j=0;j<nbEdgeOther;++j)
		{
			int next=(j+1)%nbEdgeOther;
			Vecteur v=normalize(other.pointList[next]-other.pointList[j]);
			penetration=v^(pointList[i]+position-other.pointList[j]-other.position);
			if (penetration<0.0)
			{
				in=false;
				break;
			}
			else if (penetration>subBestPenetration)
			{
				subBestPenetration=penetration;
				subBestNormale=v;
				subBestPosition=pointList[i]+position;
			}
		}
		if (in)
		{
			if (subBestPenetration>bestPenetration)
			{
				bestPenetration=subBestPenetration;
				bestNormale=subBestNormale;
				bestPosition=subBestPosition;
			}
		}
	}
	if (bestPenetration>0.0)
	{
		Collision c;
		c.isCollision=true;
		c.direction=bestPenetration*normalize(bestNormale);
		c.position=bestPosition;
		return c;
	}
	else
	{
		if (testOther) return other.isColliding(*this,false);
		else return Collision();	
	}
}

void Body::addPoint(Vecteur v)
{
	pointList.push_back(v);
}
