#ifndef ANIMATION_251E8IO9

#define ANIMATION_251E8IO9

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "animationPart.h"

class String;

class Animation
{
	private:
		float duration;
		std::string name;
		std::vector<AnimationPart> part;
		void solveKeyImage();
		std::string planche;
	public:
		void loadFromFile(std::string filename);
		void draw(float x, float y ,float time, sf::RenderWindow& screen);
};


#endif /* end of include guard: ANIMATION_251E8IO9 */
