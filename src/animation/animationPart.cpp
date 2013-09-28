#include "animationPart.h"
#include "keyImage.h"
#include <algorithm>
#include <iostream>

void AnimationPart::draw(float x, float y, float time, sf::Sprite& spr,sf::RenderWindow& screen)
{
	bool right=false;
	int n=keyimages.size();
	if (n==0) return;
	int pos=0;
	KeyImage k;
	while(true)
	{
		if (pos==n) break;
		if (keyimages[pos].time>=time)
		{
			right=true;
			break;
		}
		pos++;
	}
	if (not right)
	{
		k=keyimages[n-1];
	}
	else if ((pos-1)==-1)
	{
		k=keyimages[0];
	}
	else
	{
		k=keyimages[pos-1].interpolateWith(time,keyimages[pos]);
	}
	k.draw(x,y,spr,screen);
}
bool AnimationPart::solveKeyImage()
{

	float time=-1.0;
	int i;
	int j;
	int n=keyimages.size();
	for(i=0;i<n;++i)
	{
		if (keyimages[i].time==time)
		{
			keyimages[j].combine(keyimages[i],KeyImage::All);
			keyimages.erase(keyimages.begin()+i);
			n=keyimages.size();
			i=j;
		}
		else
		{
			j=i;
			time=keyimages[i].time;
		}
	}

	n=keyimages.size();
	for(int i=0;i<n;++i)
	{
		for(int attribute=1;attribute<=DEFINEDMASKLAST;attribute<<=1)
		{
			if (not (keyimages[i].definedMask&attribute))
			{
				KeyImage addition;
				bool left=false;
				bool right=false;
				int posleft=i;
				int posright=i;
				while(true)
				{
					posleft--;
					if (posleft<0) break;
					if (keyimages[posleft].definedMask&attribute)
					{
						left=true;
						break;
					}
				}
				while(true)
				{
					posright++;
					if (posright>=n) break;
					if (keyimages[posright].definedMask&attribute)
					{
						right=true;
						break;
					}
				}
				using namespace std;
				if (left and right)
				{
					addition=keyimages[posleft];
					float t=keyimages[i].time;
					addition=addition.interpolateWith(t,keyimages[posright]);
				}
				else if (left)
				{
					addition=keyimages[posleft];
				}
				else if (right)
				{
					addition=keyimages[posright];
				}
				keyimages[i].combine(addition,attribute);
			}
		}
	}
	return true;
}
void AnimationPart::addKeyImage(KeyImage& k)
{
	keyimages.push_back(k);
}
