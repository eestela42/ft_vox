#include "../../../includes/classes/Game/skyBox.hpp"
# include <cmath>


skyBox::skyBox() : renderDistance(1250)
{	
}

skyBox::~skyBox()
{
}

void skyBox::setRenderDistance(unsigned int renderDistance)
{
	this->renderDistance = renderDistance;

}

void skyBox::replace(int x, int y)
{
	data.clear();
	data.resize(9);
	int size = 500;
	data[0] = x + renderDistance;
	data[1] = 0;
	data[2] = 0;
	data[3] = x + sqrt(pow(data[0], 2) + size);
	data[4] = size;
	data[5] = 0;

	data[6] = x + sqrt(pow(data[0], 2) + size);
	data[7] = 0;
	data[8] = size;
}