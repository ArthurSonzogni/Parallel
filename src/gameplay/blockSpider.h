#ifndef BLOCKSPIDER_QTSUFVM9
#define BLOCKSPIDER_QTSUFVM9

#include "object.h"
#include <vector>

class BlockSpider : public Object
{
	public:
		BlockSpider(int x, int y, int width, int height);
		std::vector<Body*> getBodies();
		void draw(sf::RenderWindow& screen);
		void step();
	private:
		Body body;
	
};

#endif /* end of include guard: BLOCKSPIDER_QTSUFVM9 */
