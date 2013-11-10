#include "manager.h"
#include <map>

std::map<std::string,sf::Texture> imageContainer;

sf::Texture& getTexture(std::string& s)
{
	std::map<std::string,sf::Texture>::iterator it;
	it=imageContainer.find(s);
	if (it==imageContainer.end())
	{
		imageContainer[s].loadFromFile(s);
		imageContainer[s].setSmooth(true);
		return imageContainer[s];
	}
	else
	{
		return it->second;
	}
}
