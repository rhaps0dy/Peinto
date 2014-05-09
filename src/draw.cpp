#include "platform.h"
#include "framework.h"
#include "image.h"
#include "draw.h"
#include <malloc.h>

#define PAINTPIXEL(P) img->setPixel(P.x, P.y, c);

#define STRAIGHTFOR(comp, dir, coord) {\
	for(; p1.coord comp p2.coord ; p1.coord dir##dir) \
		PAINTPIXEL(p1) \
	PAINTPIXEL(p2) \
}

#define INTERCHANGE(A, B) { \
	A ^= B; \
	B ^= A; \
	A ^= B; \
}

#define DIAGONALFOR(dir) {\
	for(; p1.x<p2.x; p1.x++, p1.y dir##dir) \
		PAINTPIXEL(p1) \
	PAINTPIXEL(p2) \
}

#define BRESENHAM(coordA, coordB, comp, dirA, dirB) {\
	for(; p1.coordA comp p2.coordA; p1.coordA dirA##dirA) { \
		PAINTPIXEL(p1) \
		e += d##coordB ; \
		if((e+ d##coordB)*2 > d##coordA) { \
			e -= d##coordA ; \
			p1.coordB dirB##dirB ; \
		} \
	} \
	PAINTPIXEL(p2) \
}

#define BRESENHAMX(dir) BRESENHAM(x, y, <, +, dir)
#define BRESENHAMY(comp, dir) BRESENHAM(y, x, comp, dir, +)

void drawLine(Image *img, Pos2 p1, Pos2 p2, const Color c)
{
	int dx, dy, e=0;

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

	dx = (int) p2.x - p1.x;
	if(p1.y > p2.y)
	{
		dy = (int) p1.y - p2.y;
		if(dx==dy)        DIAGONALFOR(-)
		else if(dx>dy)    BRESENHAMX(-)
		else              BRESENHAMY(>, -)
	}
	else
	{
		dy = (int) p2.y - p1.y;
		if(dx==dy)        DIAGONALFOR(+)
		else if(dx>dy)    BRESENHAMX(+)
		else              BRESENHAMY(<, +)
	}
}

#define PAINTQUADRANT(op1, op2) { \
		p2.x = p.x op1 x; \
		p2.y = p.y op2 y; \
		if(p2.x < limx && p2.y < limy) \
		PAINTPIXEL(p2); \
		p2.x = p.x op1 y; \
		p2.y = p.y op2 x; \
		if(p2.x < limx && p2.y < limy) \
		PAINTPIXEL(p2); \
}

void drawCircle(Image *img, Pos2 p, Uint r, const Color c, Uint limx, Uint limy)
{
	int x, y, e;
	Pos2 p2;
	x = (int)r;
	y = 0;
	e = 1-x;
	while(y <= x) {
		PAINTQUADRANT(+, +);
		PAINTQUADRANT(+, -);
		PAINTQUADRANT(-, +);
		PAINTQUADRANT(-, -);
		y++;
		if (e<0) {
			e += 2*y+1;
		} else {
			x--;
			e += 2*(y-x+1);
		}
	}
}

Polyline *
newPolyline(Pos2 pos)
{
	Polyline *p = (Polyline *) malloc(sizeof(Polyline));
	p->f = p->l = (LPos2 *) malloc(sizeof(LPos2));
	p->f->p = pos;
	p->f->n = NULL;
	return p;
}

void
destroyPolyline(Polyline *p)
{
	LPos2 *lp = p->f->n;
	while(p->f != p->l) {
		free(p->f);
		p->f = lp;
		lp = lp->n;
	}
	if(p->l) free(p->l);
	free(p);
}

void
addPosPolyline(Polyline *p, Pos2 pos)
{
	p->l->n = (LPos2 *) malloc(sizeof(LPos2));
	p->l = p->l->n;
	p->l->n = NULL;
	p->l->p = pos;
}

void
drawPolyline(Polyline *p, Image *img, const Color c)
{
	LPos2 *lp = p->f;
	while(lp != p->l) {
		drawLine(img, lp->p, lp->n->p, c);
		lp = lp->n;
	}
}
