#include "AABB.h"

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
