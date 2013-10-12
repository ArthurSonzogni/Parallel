#ifndef MAPLOADER_398DLYIV
#define MAPLOADER_398DLYIV

#include "tinyxml2.h"
#include <string>
#include <vector>
#include "tile.h"
#include <map>
#include <list>
#include <SFML/Graphics.hpp>

typedef std::vector<std::vector<Tile> > MapLayer;

class MapLoader
{
	public:
		MapLoader(std::string file);
		std::vector<MapLayer>& getLayerBackground();
		std::vector<MapLayer>& getLayerForeground();
		void draw(MapLayer& layer,sf::RenderWindow& screen);

	private:
		void parseMap(tinyxml2::XMLElement*);
		void parseTileset(tinyxml2::XMLElement*);
		void parseLayer(tinyxml2::XMLElement*);
		std::vector<MapLayer> foregroundLayers;
		std::vector<MapLayer> backgroundLayers;
		int currentId;
		std::list<int> gidList;
		std::map<int,int> gidToId;
		int idToGid(int gid);
		std::vector<sf::Texture*> idToImg;
		int width;
		int height;
};

#endif /* end of include guard: MAPLOADER_398DLYIV */
