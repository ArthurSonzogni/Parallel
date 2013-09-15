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
	float relativeTime=time/duration;
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

void Animation::loadFromFile(std::string filename)
{
	std::ifstream file(filename.c_str(),std::ios::in);
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
						std::cerr<<"error when reading "<<line<<std::endl;
				}
				else if (name=="duration")
				{
					if (arguments.size()==1)
					{
						float t;
						convert(arguments.front(),t);

					}
					else
						std::cerr<<"error when reading "<<line<<std::endl;
				}
				else if (name=="texture")
				{
					if (arguments.size()==1)
					{
						planche=arguments.front();
					}
					else
						std::cerr<<"error when reading "<<line<<std::endl;

				}
				else if (name=="part")
				{
					if (arguments.size()==5)
					{
													
					}
					else
						std::cerr<<"error when reading "<<line<<std::endl;
				}
			}
				
		}
		file.close();
	}
	else
	{
		std::cerr<<"unable to open "+filename<<std::endl;
	}
}
