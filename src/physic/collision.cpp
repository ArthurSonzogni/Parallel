#include "collision.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;
#include <iostream>

Collision::Collision():
	direction(0.0,0.0),
	position(0.0,0.0)
{
}


void Collision::draw(RenderWindow& screen)
{
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(position.x-5*penetration*direction.x,position.y-5*penetration*direction.y)),
		sf::Vertex(sf::Vector2f(position.x+5*penetration*direction.x,position.y+5*penetration*direction.y))
	};

	screen.draw(line, 2, sf::Lines);
}

Collision add(std::vector<Collision>& v)
{
	if (v.size()==0) return Collision();
	float sum=0.0;
	Collision retour;
	retour.position=Vecteur(0.0,0.0);
	retour.direction=Vecteur(0.0,0.0);
	retour.penetration=0.0;
	for(auto &i : v)
	{
		sum+=i.penetration;
		retour.position=retour.position+i.penetration*i.position;
		retour.direction=retour.direction+i.penetration*i.direction;
		retour.penetration+=i.penetration;
	}
	retour.position=(1.0/sum)*retour.position;
	retour.direction=normalize(retour.direction);
	retour.penetration=(1.0/double(v.size()))*retour.penetration;
	return retour;
}
