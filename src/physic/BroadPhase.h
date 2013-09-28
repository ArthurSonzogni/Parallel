#ifndef BROADPHASE_SEI59BZF
#define BROADPHASE_SEI59BZF

#include <vector>
#include "AABB.h"
#include "Body.h"

class BroadPhase
{
	public:
		BroadPhase(std::vector<Body*>& input);
		std::vector<std::vector<Body*> >* getOuput();
	private:
		std::vector<AABB> aabb;
		std::vector<std::vector<Body*>> ouput;
		std::vector<double> xList;
		std::vector<double> yList;
};

#endif /* end of include guard: BROADPHASE_SEI59BZF */
