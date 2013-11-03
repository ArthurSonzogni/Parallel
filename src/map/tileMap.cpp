#include "tileMap.h"

TileMap::TileMap()
{
	texture=NULL;
}

void TileMap::setTexture(sf::Texture& t)
{
	texture=&t;
}

vector<sf::Vertex>& TileMap::getVertices()
{
	return vertices;
}

sf::Texture* TileMap::getTexture()
{
	return texture;
}

void TileMap::draw(sf::RenderTarget& target,sf::RenderStates states) const
{
	states.transform *= getTransform();
	states.texture=texture;
	target.draw(&vertices[0],vertices.size(),Quads,states);
}
