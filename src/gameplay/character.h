#ifndef CHARACTER_4S9L8QKN
#define CHARACTER_4S9L8QKN

#include "../physic/Body.h"
#include <vector>
#include "object.h"

class Character : public Object
{
	public:
		Character();
		void draw(sf::RenderWindow& screen);
		std::vector<Body*> getBodies();
		void setPosition(Vecteur p);
		Vecteur getPosition();
		void keyLeft();
		void keyRight();
		void keyUp();
		void step();
	private:
		Body body;
		Body jumpDetector;
		void jumpDetectorResetPosition();
};

#endif /* end of include guard: CHARACTER_4S9L8QKN */
