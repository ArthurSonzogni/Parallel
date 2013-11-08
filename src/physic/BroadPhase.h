#ifndef BROADPHASE_SEI59BZF
#define BROADPHASE_SEI59BZF

#include <vector>
#include <set>
#include "AABB.h"
#include "Body.h"
#include <SFML/Graphics.hpp>

using BroadPhaseInput = std::vector<Body*>;
using BroadPhaseInputInt = std::vector<int>;
using BroadPhaseOutputInt = std::vector<std::vector<int> >;
using BroadPhaseOutput = std::set<std::pair<int,int> >;

class BroadPhase
{
	public:
		//BroadPhase(BroadPhaseInput& input,sf::RenderWindow& screen);
		BroadPhase(BroadPhaseInput& input);
		BroadPhaseOutput& getOutput();
	private:
		//sf::RenderWindow& s;
		BroadPhaseInput input;
		BroadPhaseOutput output;
		std::vector<AABB> aabb;

		BroadPhaseOutputInt split(BroadPhaseInputInt b,AABB& box, int contrat);
};


#endif /* end of include guard: BROADPHASE_SEI59BZF */
