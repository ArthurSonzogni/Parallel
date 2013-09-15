#ifndef ANIMATIONPART_JTWA6AWA

#define ANIMATIONPART_JTWA6AWA

#include <SFML/Graphics.hpp>
#include <vector>
#include "keyImage.h"

class AnimationPart
{
	private:
		std::vector<KeyImage> keyimages;	
	public:
		void draw(float x, float y, float time, sf::Sprite& spr,sf::RenderWindow& screen);
		bool solveKeyImage();
		void addKeyImage(KeyImage& k);
};


#endif /* end of include guard: ANIMATIONPART_JTWA6AWA */

