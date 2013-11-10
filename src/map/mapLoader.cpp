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
	
	// build TileMap
	for(auto &layer : backgroundLayers)
	{
		vector<TileMap> t;
		mapLayerToTileMap(layer,t);
		backgroundTileMap.insert(backgroundTileMap.end(),t.begin(),t.end());	
	}
	for(auto &layer : foregroundLayers)
	{
		vector<TileMap> t;
		mapLayerToTileMap(layer,t);
		foregroundTileMap.insert(foregroundTileMap.end(),t.begin(),t.end());	
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
			else if (value=="objectgroup")
			{
				parseObjectGroup(element);
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

void MapLoader::parseObjectGroup(tinyxml2::XMLElement* element)
{
	const char* tilesetName=element->Attribute("name");
	if (tilesetName)
	{
		if (string("fixed")==tilesetName)
			parseFixedObject(element);
	}
}

void MapLoader::parseFixedObject(tinyxml2::XMLElement* element)
{
	XMLNode* node=element->FirstChild();
	while(node)
	{
		XMLElement* element=node->ToElement();
		if (element)
		{
			string value=element->Value();
			if (value=="object")
			{
				int x,y;
				int returnX = element->QueryIntAttribute("x",&x);
				int returnY = element->QueryIntAttribute("y",&y);
				if (returnX==XML_NO_ERROR and returnY==XML_NO_ERROR)
				{
					XMLNode* node=element->FirstChild();
					while(node)
					{
						XMLElement* element=node->ToElement();
						if (element)
						{
							string value=element->Value();
							if (value=="polygon")
							{
								const char* value=element->Attribute("points");
								if (value)
								{
									unsigned int index=0;
									int buf=0;
									bool left=true;
									int xpos=0;
									bool inv=false;
									Body body;
									if (value[index]!='\0')
									for(;;)
									{
										char v=value[index];
										if (v>='0' and v<='9')
										{
											buf=10*buf+v-'0';
										}
										else if (v=='-')
										{
											inv=true;
										}
										else
										{
											if (left)
											{
												if (inv) buf=-buf;
												inv=false;
												xpos=buf;
												buf=0;
											}
											else
											{
												if (inv) buf=-buf;
												inv=false;
												cout<<"point("<<xpos<<","<<buf<<")"<<endl;
												body.addPoint(Vecteur(x+xpos,y+buf));
												buf=0;
											}
											left^=true;
											if (v=='\0')
											{
												body.reoriente();
												body.recenter();
												body.setGroup(GROUP_STATIC,GROUP_CHARACTER);
												fixedBody.push_back(body);
												cout<<"ajout"<<endl;
												break;
											}
										}
										index++;
									}
								}
							}
						}
						node = node->NextSibling();
					}
				}
			}
		}
		node = node->NextSibling();
	}
}

vector<MapLayer>& MapLoader::getLayerBackground()
{
	return backgroundLayers;
}

vector<MapLayer>& MapLoader::getLayerForeground()
{
	return foregroundLayers;
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

vector<Body>& MapLoader::getFixedBody()
{
	return fixedBody;
}
vector<TileMap>& MapLoader::getTileMapBackground()
{
	return backgroundTileMap;
}
vector<TileMap>& MapLoader::getTileMapForeground()
{
	return foregroundTileMap;
}
void MapLoader::mapLayerToTileMap(const MapLayer& input, vector<TileMap>& output)
{
	int x=0;
	int y=0;
	for(auto &line : input)
	{
		x=0;
		for(auto &c : line)
		{
			if (c.id!=0)
			{
				Texture* texture=idToImg[c.id];
				vector<TileMap>::iterator it=output.begin();
				TileMap* current;
				for(;;)
				{
					if (it==output.end())
					{
						TileMap tilemap;
						tilemap.setTexture(*texture);
						output.push_back(tilemap);
						current=&(output.back());
						break;
					}
					else if (it->getTexture()==texture)
					{
						current=&(*it);
						break;
					}
					else
					{
						it++;
					}
				}
				vector<Vertex>& vertice = current->getVertices();
				float X=x;
				float Y=y;
				float tx=c.tilesetX;
				float ty=c.tilesetY;
				vertice.push_back(Vertex(Vector2f(X,Y),Vector2f(32.0*tx,32.0*ty)));
				vertice.push_back(Vertex(Vector2f(X+32.0,Y),Vector2f(32.0*tx+32.0,32.0*ty)));
				vertice.push_back(Vertex(Vector2f(X+32.0,Y+32.0),Vector2f(32.0*tx+32.0,32.0*ty+32.0)));
				vertice.push_back(Vertex(Vector2f(X,Y+32.0),Vector2f(32.0*tx,32.0*ty+32.0)));
			}
			x+=32;
		}
		y+=32;
	}
}
