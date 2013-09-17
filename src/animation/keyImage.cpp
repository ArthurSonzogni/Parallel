#include <SFML/System.hpp>
#include "keyImage.h"

KeyImage::KeyImage()
{
	time=0.0;
	definedMask=None;
	textureCoord=sf::Rect<int>(0,0,0,0);
	center=sf::Vector2<int>(0.0,0.0);
	position=sf::Vector2<float>(0.0,0.0);
	rotation=0.0;
}
void KeyImage::setTime(float t)
{
	time=t;
}
void KeyImage::setTextureCoord(sf::Rect<int> text,sf::Vector2<int> c)
{
	textureCoord=text;
	center=c;
	definedMask|=TextureCoord;
}
void KeyImage::setPosition(sf::Vector2<float> pos)
{
	position=pos;
	definedMask|=Position;
}
void KeyImage::setRotation(float r)
{
	rotation=r;
	definedMask|=Rotation;
}
bool operator<(KeyImage a, KeyImage b)
{
	return a.time<b.time;
}
float KeyImage::getTime()
{
	return time;
}
#include <iostream>
KeyImage KeyImage::interpolateWith(float t,KeyImage& other)
{
	KeyImage k;
	k.setTime(t);
	float ratio=(t-time)/(other.time-time);
	float invratio=1.0-ratio;
	if ((definedMask&TextureCoord))
	{
		k.setTextureCoord(textureCoord,center);
	}
	if ((definedMask&Position) and (other.definedMask&Position))
	{	
		k.setPosition(position*invratio+(other.position)*ratio);
	}
	if ((definedMask&Rotation) and (other.definedMask&Rotation))
	{
		using namespace std;
		k.setRotation(rotation*invratio+other.rotation*ratio);
	}

	return k;
}
void KeyImage::combine(KeyImage& k,int autorizedMask)
{
	if (autorizedMask&TextureCoord)
	if (not (definedMask&TextureCoord))
	if ( (k.definedMask&TextureCoord))
		setTextureCoord(k.textureCoord,k.center);

	if (autorizedMask&Position)
	if (not (definedMask&Position))
	if ((k.definedMask&Position))
		setPosition(k.position);

	if (autorizedMask&Rotation)
	if (not (definedMask&Rotation))
	if ((k.definedMask&Rotation))
		setRotation(k.rotation);
}

#include <iostream>
void KeyImage::draw(float x, float y, sf::Sprite& spr, sf::RenderWindow& screen)
{
	spr.setPosition(position+sf::Vector2<float>(x,y));
	spr.setRotation(rotation);
	spr.setTextureRect(textureCoord);
	spr.setOrigin(center.x,center.y);
	screen.draw(spr);
}
