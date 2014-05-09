#include "image.h"
#include "framework.h"
#include "bresenham.h"

#define PAINTPIXEL img->setPixel(p1.x, p1.y, *c);

#define STRAIGHTFOR(comp, dir, coord) {\
	for(; p1.coord comp##= p2.coord ; p1.coord dir##dir) \
		PAINTPIXEL \
}

#define INTERCHANGE(A, B) { \
	A ^= B; \
	B ^= A; \
	A ^= B; \
}

#define DIAGONALFOR(dir) {\
	for(; p1.x<=p2.x; p1.x++, p1.y dir##dir) \
		PAINTPIXEL \
}

#define BRESENHAM(coordA, coordB, comp, dirA, dirB) {\
	for(; p1.coordA comp##= p2.coordA; p1.coordA dirA##dirA) \
	{ \
		PAINTPIXEL \
		if(((e+ d##coordB )<<1) < d##coordA ) \
			e += d##coordB ; \
		else \
		{ \
			p1.coordB dirB##dirB ; \
			e += d##coordB - d##coordA ; \
		} \
	} \
}

#define BRESENHAMX(dir) BRESENHAM(x, y, <, +, dir)
#define BRESENHAMY(comp, dir) BRESENHAM(y, x, comp, dir, +)

void drawLine(Image *img, Pos2 p1, Pos2 p2, const Color *c)
{
	Uint dx, dy, e=0;

	img->setPixel(p1.x, p1.y, Color::GREEN);
	img->setPixel(p2.x, p2.y, Color::BLUE);

	if(p1.y==p2.y)
	{
		if(p1.x < p2.x) STRAIGHTFOR(<, +, x)
		else            STRAIGHTFOR(>, -, x)
		return;
	}
	if(p1.x==p2.x)
	{
		if(p1.y < p2.y) STRAIGHTFOR(<, +, y)
		else            STRAIGHTFOR(>, -, y)
		return;
	}

	//make sure p1 has the lowest x
	if(p2.x < p1.x)
	{
		INTERCHANGE(p1.x, p2.x)
		INTERCHANGE(p1.y, p2.y)
	}

	dx = p2.x - p1.x;
	if(p1.y > p2.y)
	{
		dy = p1.y - p2.y;
		if(dx==dy)        DIAGONALFOR(-)
		else if(dx>dy)    BRESENHAMX(-)
		else              BRESENHAMY(>, -)
	}
	else
	{
		dy = p2.y - p1.y;
		if(dx==dy)        DIAGONALFOR(+)
		else if(dx>dy)    BRESENHAMX(+)
		else              BRESENHAMY(<, +)
	}
}
void drawCircle(Image *img, Pos2 p, Uint r, const Color *c)
{
	;
}
