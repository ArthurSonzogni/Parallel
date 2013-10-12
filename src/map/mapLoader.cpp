#include "mapLoader.h"
#include "../ressource/manager.h"
#include <iostream>
#include <exception>

using namespace std;
using namespace sf;
using namespace tinyxml2;

MapLoader::MapLoader(string file)
{
	currentId=1;
	gidList.push_back(-1);
	idToImg.push_back(NULL);
	XMLDocument doc;
	doc.LoadFile(file.c_str());

	// some optimization
	backgroundLayers.reserve(10);
	foregroundLayers.reserve(10);

	XMLNode* node = doc.FirstChild();
	while(node)
	{
		XMLElement* element=node->ToElement();
		if (element)
		{
			string value=element->Value();	
			if (value=="map")
			{
				parseMap(element);
			}
		}
		node = node->NextSibling();
	}
}

void MapLoader::parseMap(XMLElement* element)
{
	
	int widthRef,heightRef;
	int widthReturn = element->QueryIntAttribute("width",&widthRef);
	int heightReturn = element->QueryIntAttribute("height",&heightRef);
	if (widthReturn == XML_NO_ERROR and heightReturn == XML_NO_ERROR)
	{
		width=widthRef;
		height=heightRef;
	}
	else
	{
		throw exception();
	}

	XMLNode* node=element->FirstChild();
	while(node)
	{
		XMLElement* element=node->ToElement();
		if (element)
		{
			string value=element->Value();
			if (value=="tileset")
			{
				parseTileset(element);
			}
			else if (value=="layer")
			{
				parseLayer(element);
			}
		}
		node = node->NextSibling();
	}
}

void MapLoader::parseTileset(XMLElement* element)
{
	const char* imageName=element->Attribute("name");
	int gid;
	int gidReturn = element->QueryIntAttribute("firstgid",&gid);
	if (imageName && gidReturn==XML_NO_ERROR)
	{
		string image("./img/");
		image+=imageName;
		idToImg.push_back(&getTexture(image));
		gidList.push_back(gid);
		gidToId[gid]=currentId;
		currentId++;
	}
		
	//XMLNode* node=element->FirstChild();
	//while(node)
	//{
		//XMLElement* element=node->ToElement();
		//if (element)
		//{
			//string value=element->Value();
			//if (value=="image")
			//{

			//}
		//}
		//node = node->NextSibling();
	//}
}

void MapLoader::parseLayer(XMLElement* element)
{
	const char* tilesetName=element->Attribute("name");
	if (tilesetName)
	{
		MapLayer layer;
		layer.reserve(height);
		for(int i=0;i<height;++i)
		{
			layer.push_back(vector<Tile>());
			layer[i].reserve(width);
			for(int j=0;j<width;++j)
			{
				layer[i].push_back(Tile(0,0,0));
			}
		}
		
		XMLNode* node=element->FirstChild();
		while(node)
		{
			XMLElement* element=node->ToElement();
			if (element)
			{
				string value=element->Value();
				if (value=="data")
				{
					const char* text=element->GetText();
					if (text)
					{
						int line=0;
						int column=0;
						int current=0;
						bool okNumber=false;
						while(*text)
						{
							char b=*text;
							if (b>='0' and b<='9')
							{
								current=10*current+int(b-'0');
								okNumber=true;
							}
							else
							{
								if (okNumber)
								{
									if (current!=0)
									{
										int gid=idToGid(current);
										int id=gidToId[gid];
										int tilesetWidth=idToImg[id]->getSize().x/32;
										int tilesetX=(current-gid)%tilesetWidth;
										int tilesetY=(current-gid)/tilesetWidth;
										layer.at(line).at(column).id=id;
										layer.at(line).at(column).tilesetX=tilesetX;
										layer.at(line).at(column).tilesetY=tilesetY;
									}

									okNumber=false;
									current=0;

									column++;
									if (column>=width)
									{
										column=0;
										line++;
									}
								}
							}
							text++;
						}
					}
				}
			}
			node = node->NextSibling();
		}

		if (tilesetName[0]=='b')
		{
			backgroundLayers.push_back(layer);
		}
		else if (tilesetName[0]=='f')
		{
			foregroundLayers.push_back(layer);
		}
	}
}


vector<MapLayer>& MapLoader::getLayerBackground()
{
	return foregroundLayers;
}

vector<MapLayer>& MapLoader::getLayerForeground()
{
	return backgroundLayers;
}

int MapLoader::idToGid(int gid)
{
	int i=0;
	auto it=gidList.begin();
	while(it!=gidList.end() and (*it)<=gid)
	{
		++it;
		++i;
	}
	return i-1;
}

void MapLoader::draw(MapLayer& layer,sf::RenderWindow& screen)
{
	int x=0;
	int y=0;
	Sprite sprite;
	sprite.setTextureRect(IntRect(0,0,32,32));
	int previousId=-1;
	for(auto &line : layer)
	{
		x=0;
		for(auto &c : line)
		{
			if (c.id!=0)
			{
				if (previousId!=c.id)
				{
					previousId=c.id;
					sprite.setTexture(*idToImg[c.id]);
				}
				sprite.setPosition(x,y);
				sprite.setTextureRect(IntRect(32*c.tilesetX,32*c.tilesetY,32,32));
				screen.draw(sprite);
			}
			x+=32;
		}
		y+=32;
	}
}
