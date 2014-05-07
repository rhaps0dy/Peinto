#include "bresenham.h"

void drawLine(Image *img, Pos p1, Pos p2, Color c)
{
	//special cases, inlined
	if(p1.y==p2.y)
	{
		if(p1.x < p2.x)
			for(; p1.x<=p2.x; p1.x++)
				img->setPixel(p1.x, p1.y, c);
		else
			for(; p1.x>=p2.x; p1.x--)
				img->setPixel(p1.x, p1.y, c);
		return;
	}
	if(p1.x==p2.x)
	{
		if(p1.y < p2.y)
			for(; p1.y<=p2.y; p1.y++)
				img->setPixel(p1.x, p1.y, c);
		else
			for(; p1.y>=p2.y; p1.y--)
				img->setPixel(p1.x, p1.y, c);
		return;
	}

	//right half
	if(p1.x < p2.x)
		//first quadrant
		if(p1.y > p2.y)

		else
	//left half
	else

}
void drawCircle(Image *img, Pos p, Uint r, Color c)
{
	;
}
