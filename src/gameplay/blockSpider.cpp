#include "blockSpider.h"
#include <iostream>
using namespace std;

BlockSpider::BlockSpider(int x, int y, int width, int height)
{
	
}

vector<Body*> BlockSpider::getBodies()
{
	vector<Body*> retour;
	return retour;
}

void BlockSpider::draw(sf::RenderWindow& screen)
{
	cout<<"Je me draw [spider]"<<endl;
}

void BlockSpider::step()
{
	cout<<"Je me step [spider]"<<endl;
}
