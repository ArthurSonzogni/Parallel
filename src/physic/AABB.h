#ifndef AABB_VEOSHWTT
#define AABB_VEOSHWTT

#include <SFML/Graphics.hpp>

class AABB
{
	public:
		double minx,maxx,miny,maxy;
		AABB();
		AABB(double Minx,double Maxx,double Miny,double Maxy);

		void mix(AABB& other);
		bool intersect(AABB& other);
		void draw(sf::RenderWindow& screen);
		
		void split(AABB& out1, AABB& out2);
};


#endif /* end of include guard: AABB_VEOSHWTT */
