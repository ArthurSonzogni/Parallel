#include "BroadPhase.h"

using namespace std;
BroadPhase::BroadPhase(vector<Body*>& input)
{
	// generate AABB list, xList, yList
	aabb.reserve(input.size());
	xList.reserve(2*input.size());
	yList.reserve(2*input.size());
	for(auto &b : input)
	{
		aabb.push_back(b->getAABB());
		xList.push_back(aabb.back().minx);
		xList.push_back(aabb.back().maxx);
		yList.push_back(aabb.back().miny);
		yList.push_back(aabb.back().maxy);
	}
}
