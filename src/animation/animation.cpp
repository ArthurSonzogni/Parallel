#include "animation.h"
#include "animationPart.h"
#include "../ressource/manager.h"
#include <list>
#include <string>
#include <iostream>
#include <fstream>
#include "../utils/stringConvertion.h"

void Animation::draw(float x, float y ,float time, sf::RenderWindow& screen)
{
	std::vector<AnimationPart>::iterator it;
	float relativeTime=100*time/(duration);
	sf::Sprite spr;
	spr.setTexture(getTexture(planche));
	for(it=part.begin();it!=part.end();++it)
	{
		it->draw(x,y,relativeTime,spr,screen);
	}
}


// helper function for loadFromFile
bool functionStyleParse(std::string& input,std::string& name, std::list<std::string>& args)
{
	int n=input.size();
	int i=0;
	int mode=0;
	int argBegin=0;
	while(true)
	{
		if (i==n) return false;
		char c=input[i];
		if (mode==0)
		{
			if (c=='(')
			{
				name=input.substr(0,i);
				argBegin=i+1;
				mode=1;
			}
		}
		else if (mode==1)
		{
			if (c==',' or c==')')
			{
				args.push_back(input.substr(argBegin,i-argBegin));
				argBegin=i+1;

				if (c==')')
				{
					return true;
				}
			}
		}
		i++;
	}
}

void error(std::string& line, int &lineNb)
{
	std::cerr<<"error when reading "<<line<<" at "<<lineNb<<std::endl;
}

void Animation::loadFromFile(std::string filename)
{
	std::ifstream file(filename.c_str(),std::ios::in);

	std::map<std::string,int> partId;
	int nbPart=0;
	
	float currentTime=0.0;
	int lineNb=1;

	if (file)
	{
		std::string line;
		
		while(getline(file,line))
		{
			std::string name;
			std::list<std::string> arguments;
			if (functionStyleParse(line,name,arguments))
			{
				if (name=="name")
				{
					if (arguments.size()==1)
						name=arguments.front();
					else
						error(line,lineNb);
				}
				else if (name=="duration")
				{
					if (arguments.size()==1)
					{
						float t;
						if(convert(arguments.front(),t))
						duration=t;
					}
					else
						std::cerr<<"error when reading "<<line<<" at "<<lineNb<<std::endl;
				}
				else if (name=="board")
				{
					if (arguments.size()==1)
					{
						planche=arguments.front();
					}
					else
						error(line,lineNb);

				}
				else if (name=="part")
				{
					if (arguments.size()==1)
					{
						std::string partname=arguments.front();
						partId[partname]=nbPart;
						AnimationPart p;
						part.push_back(p);
						nbPart++;
					}
					else
						error(line,lineNb);
				}
				else if (name=="time")
				{
					if (arguments.size()==1)
					{
						int t;
						if (convert(arguments.front(),t))
						{
							currentTime=t;
						}
						else
							error(line,lineNb);
					}
					else
						error(line,lineNb);
				}
				else if (name=="texture")
				{
					if (arguments.size()==7)
					{
						std::string partname=arguments.front();
						int partx, party, partxsize, partysize, centerx, centery;
						std::list<std::string>::iterator it=arguments.begin();
						bool ok=true;
							ok&=convert(*(++it),partx);
							ok&=convert(*(++it),party);
							ok&=convert(*(++it),partxsize);
							ok&=convert(*(++it),partysize);
							ok&=convert(*(++it),centerx);
							ok&=convert(*(++it),centery);
						if (not ok)
							error(line,lineNb);
						else
						{
							std::map<std::string,int>::iterator it;
							it=partId.find(partname);
							if (it!=partId.end())
							{
								int id=it->second;
								KeyImage kimg;
								kimg.setTime(currentTime);
								sf::Rect<int> r(partx,party,partxsize,partysize);
								sf::Vector2<int> c(centerx,centery);
								kimg.setTextureCoord(r,c);
								part[id].addKeyImage(kimg);
							}
							else
								error(line,lineNb);
						}
					}
					else
						error(line,lineNb);
				}
				else if (name=="position")
				{
					if (arguments.size()==3)
					{
						std::string partname=arguments.front();
						float partx, party;
						std::list<std::string>::iterator it=arguments.begin();
						bool ok=true;
							ok&=convert(*(++it),partx);
							ok&=convert(*(++it),party);
						if (not ok)
							error(line,lineNb);
						else
						{
							std::map<std::string,int>::iterator it;
							it=partId.find(partname);
							if (it!=partId.end())
							{
								int id=it->second;
								KeyImage kimg;
								kimg.setTime(currentTime);
								sf::Vector2<float> pos(partx,party);
								kimg.setPosition(pos);
								part[id].addKeyImage(kimg);
							}
							else
								error(line,lineNb);
						}
					}
					else
						error(line,lineNb);
				}
				else if (name=="angle")
				{
					if (arguments.size()==2)
					{
						std::string partname=arguments.front();
						float angle;
						std::list<std::string>::iterator it=arguments.begin();
						bool ok=true;
							ok&=convert(*(++it),angle);
						if (not ok)
							error(line,lineNb);
						else
						{
							std::map<std::string,int>::iterator it;
							it=partId.find(partname);
							if (it!=partId.end())
							{
								int id=it->second;
								KeyImage kimg;
								kimg.setTime(currentTime);
								kimg.setRotation(angle);
								part[id].addKeyImage(kimg);
							}
							else
								error(line,lineNb);
						}
					}
					else
						error(line,lineNb);
				}
			}



			lineNb++;
		}
		file.close();
		
		solveKeyImage();
	}
	else
	{
		std::cerr<<"unable to open "+filename<<std::endl;
	}
}

void Animation::solveKeyImage()
{
	for(unsigned int i=0;i<part.size();++i)
	{
		part[i].solveKeyImage();
	}
}
