#ifndef RING_4CXNUE3D
#define RING_4CXNUE3D

#include <SFML/Graphics.hpp>
#include <vector>


class Ring
{
	enum RingElement
	{
		Death,
		Punch,
		DoubleJump,
		Mini,
		SpeedUp,
		Hammer,
		Nothing
	};
	
	public:
		void draw(sf::RenderWindow& screen);	
	private:
		std::vector<RingElement> elements;
};


#endif /* end of include guard: RING_4CXNUE3D */
