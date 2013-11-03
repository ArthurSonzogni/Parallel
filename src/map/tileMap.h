#ifndef TILEMAP_J6CMG4EF
#define TILEMAP_J6CMG4EF

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class TileMap : public sf::Drawable, public sf::Transformable
{
	public:
		TileMap();
		void setTexture(sf::Texture& t);
		std::vector<sf::Vertex>& getVertices();
		sf::Texture* getTexture();
	private:
		std::vector<sf::Vertex> vertices;
		sf::Texture* texture;
		virtual void draw(sf::RenderTarget& target,sf::RenderStates states) const;
};

#endif /* end of include guard: TILEMAP_J6CMG4EF */
