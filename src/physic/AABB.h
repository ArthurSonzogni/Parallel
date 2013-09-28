#ifndef AABB_VEOSHWTT
#define AABB_VEOSHWTT

class AABB
{
	public:
		double minx,maxx,miny,maxy;
		AABB(double Minx,double Maxx,double Miny,double Maxy);

		void mix(AABB& other);
		bool intersect(AABB& other);
};


#endif /* end of include guard: AABB_VEOSHWTT */
