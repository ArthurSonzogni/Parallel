#ifndef KEYIMAGE_S03CL0G5

#define KEYIMAGE_S03CL0G5

#include <SFML/Graphics.hpp>

#define DEFINEDMASKLAST 4
class KeyImage
{
	public:
	enum DefinedMask
	{
		None=0,
		TextureCoord=1,
		Position=2,
		Rotation=4,
		All=7
	};


	public:
		float time;
		int definedMask;
		sf::Rect<int> textureCoord;
		sf::Vector2<int> center;
		sf::Vector2<float> position;
		float rotation;

	public:
		KeyImage();
		void setTime(float time);
		float getTime();
		void setTextureCoord(sf::Rect<int> text,sf::Vector2<int> center);
		void setPosition(sf::Vector2<float> pos);
		void setRotation(float r);
		
		KeyImage interpolateWith(float t,KeyImage& other);
		void combine(KeyImage& k, int autorizedMask);
		void draw(float x, float y, sf::Sprite& spr, sf::RenderWindow& screen);
};
bool operator<(KeyImage a, KeyImage b);


#endif /* end of include guard: KEYIMAGE_S03CL0G5 */
