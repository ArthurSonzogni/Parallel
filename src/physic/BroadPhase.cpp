#include "BroadPhase.h"
#include <algorithm>

#include <iostream>
using namespace std;

BroadPhase::BroadPhase(BroadPhaseInput& Input,sf::RenderWindow& screen):
	s(screen),
	input(Input)
{
	// generate AABB list
	aabb.reserve(input.size());
	for(auto &b : input)
	{
		aabb.push_back(b->getAABB());
	}
	
	// mix all AABB
	AABB allAABB=aabb[0];
	for(auto &i:aabb)
	{
		allAABB.mix(i);
	}

	allAABB.draw(screen);

	// vector select all
	BroadPhaseInputInt selectorAll;
	selectorAll.reserve(Input.size());
	for(unsigned int i=0;i<input.size();++i)
		selectorAll.push_back(i);

	BroadPhaseOuputInt outputInt= split(selectorAll,allAABB,Input.size());
	
	for(auto &group : outputInt)
	{
		for(auto element=group.begin();element!=group.end();++element)
		{
			for(auto other=element+1;other!=group.end();++other)
			{
				output.insert(pair<int,int>(*element,*other));
			}
		}
	}
}


BroadPhaseOuputInt BroadPhase::split(BroadPhaseInputInt b,AABB& box, int contrat)
{
	if (b.size()<=1)
	{
		BroadPhaseOuputInt r;
		return r;
	}
	else if (contrat==0)
	{
		BroadPhaseOuputInt r;
		r.push_back(b);
		return r;
	}
	else
	{
		AABB firstAABB,secondAABB;
		box.split(firstAABB,secondAABB);

		vector<int> input1;
		vector<int> input2;
		for(auto i : b)
		{
			if (aabb[i].intersect(firstAABB))
				input1.push_back(i);
			if (aabb[i].intersect(secondAABB))
				input2.push_back(i);
		}
		
		// test if splitting is good
		int superflue=int(input1.size()+input2.size())-int(b.size());
		if (superflue > contrat || superflue>=int(b.size()))
		{
			BroadPhaseOuputInt r;
			r.push_back(b);
			return r;
		}
		else
		{
			BroadPhaseOuputInt out1=split(input1,firstAABB,contrat*9/10);
			BroadPhaseOuputInt out2=split(input2,secondAABB,contrat*9/10);

			firstAABB.draw(s);
			secondAABB.draw(s);
			
			out1.reserve(out1.size()+out2.size());	
			out1.insert(out1.end(),out2.begin(),out2.end());
			return out1;
		}
	}
}

BroadPhaseOuput& BroadPhase::getOuput()
{
	return output;
}

