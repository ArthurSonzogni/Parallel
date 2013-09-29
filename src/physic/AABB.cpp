#include "AABB.h"

AABB::AABB():
	minx(0.0),
	maxx(0.0),
	miny(0.0),
	maxy(0.0)
{

}

void AABB::mix(AABB& other)
{
	if (other.minx<minx) minx=other.minx;
	if (other.miny<miny) miny=other.miny;
	if (other.maxx>maxx) maxx=other.maxx;
	if (other.maxy>maxy) maxy=other.maxy;
}

bool AABB::intersect(AABB& other)
{
	if (minx>other.maxx or other.minx>maxx or miny>other.maxy or other.miny>maxy)
		return false;
	else
		return true;
}

AABB::AABB(double Minx,double Maxx,double Miny,double Maxy):
	minx(Minx),
	maxx(Maxx),
	miny(Miny),
	maxy(Maxy)
{

}

void AABB::draw(sf::RenderWindow& screen)
{
	sf::ConvexShape c;
	c.setPointCount(4);

	c.setPoint(0,sf::Vector2f(minx,miny));
	c.setPoint(1,sf::Vector2f(maxx,miny));
	c.setPoint(2,sf::Vector2f(maxx,maxy));
	c.setPoint(3,sf::Vector2f(minx,maxy));

	c.setOutlineColor(sf::Color(255,255,255,255));
	c.setOutlineThickness(1);
	c.setFillColor(sf::Color(255,100,255,20));
	screen.draw(c);
}


void AABB::split(AABB& out1, AABB& out2)
{
	double dx,dy;
	dx=maxx-minx;	
	dy=maxy-miny;
	if (dx>dy)
	{
		out1=AABB(minx,minx+dx*0.5,miny,maxy);
		out2=AABB(minx+dx*0.5,maxx,miny,maxy);
	}
	else
	{
		out1=AABB(minx,maxx,miny,miny+dy*0.5);
		out2=AABB(minx,maxx,miny+dy*0.5,maxy);
	}
}
