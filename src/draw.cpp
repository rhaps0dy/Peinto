#include "platform.h"
#include <math.h>
#include "framework.h"
#include "image.h"
#include "draw.h"
#include <malloc.h>
#include <stdio.h>

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
		if(p2.x < img->width && p2.y < img->height) \
		PAINTPIXEL(p2); \
		p2.x = p.x op1 y; \
		p2.y = p.y op2 x; \
		if(p2.x < img->width && p2.y < img->height) \
		PAINTPIXEL(p2); \
}

void drawCircle(Image *img, Pos2 p, Uint r, const Color c)
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

LPos2 *
LPos2New(void)
{
	LPos2 *lp = (LPos2 *) malloc(sizeof(LPos2));
	lp->n = NULL;
	return lp;
}

LLPos2 *
LLPos2New(void)
{
	LLPos2 *p = (LLPos2 *) malloc(sizeof(LLPos2));
	p->f = p->l = LPos2New();
	return p;
}

void
LLPos2Destroy(LLPos2 *p)
{
	int total=0;
	while(popLLPos2(p))total++;
	free(p->f);
	free(p);
}

void
LLPos2Add(LLPos2 *p, Pos2 pos)
{
	p->l->n = LPos2New();
	p->l->p = pos;
	p->l = p->l->n;
}

void
LLPos2Draw(LLPos2 *p, Image *img, const Color c)
{
	LPos2 *lp = p->f;
	while(lp->n != p->l) {
		drawLine(img, lp->p, lp->n->p, c);
		lp = lp->n;
	}
}

char
popLLPos2(LLPos2 *p)
{
	LPos2 *lp;
	lp = p->f;
	if(!lp->n)
		return 0;
	p->f = lp->n;
	free(lp);
	return -1;
}

void
fill(Image *img, Pos2 pos, const Color c)
{
	Color init;
	Uint x, y;
	LLPos2 *p = LLPos2New();
	init = img->getPixel(pos.x, pos.y);
	LLPos2Add(p, pos);
	while(1) {
		x = p->f->p.x;
		y = p->f->p.y;
		if(!popLLPos2(p)) return;
		if(img->getPixel(x, y) != init) continue;

		img->setPixel(x, y, c);
		if(x+1 < img->width) LLPos2Add(p, Pos2(x+1, y));
		if(x-1 < img->width) LLPos2Add(p, Pos2(x-1, y));
		if(y+1 < img->height) LLPos2Add(p, Pos2(x, y+1));
		if(y-1 < img->height) LLPos2Add(p, Pos2(x, y-1));
	}
	LLPos2Destroy(p);
}

#ifndef RELEASE
void
printLLPos2Length(LLPos2 *p)
{
	int i=0;
	LPos2 *lp;
	for(lp = p->f; lp != p->l; lp = lp->n) i++;
	printf("Length %d, Head %x, Tail %x\n", i, p->f, p->l);
}

void
printLLPos2(LLPos2 *p)
{
	LPos2 *lp = p->f;
	printf("First: %x, Last: %x\n", p->f, p->l);
	while(lp->n) {
		printf("[%u %u %x %x] -> ", lp->p.x, lp->p.y, lp, lp->n);
		lp = lp->n;
	}
	printf("[%x]\n\n", lp->n);
}
#endif
