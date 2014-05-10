#include "platform.h"
#include <math.h>
#include "framework.h"
#include "image.h"
#include "draw.h"
#include <malloc.h>
#include <stdio.h>
#include <stdint.h>

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

#define ADDTOQUEUEX(oper) { \
	arrpos = y*w + (x oper 1)/8; \
	mask = 0x01<<((x oper 1)%8); \
	if((x oper 1) < img->width && !(visited[arrpos] & mask)) { \
		visited[arrpos] |= mask; \
		LLPos2Add(p, Pos2(x oper 1, y)); \
	} \
}

#define ADDTOQUEUEY(oper) { \
	arrpos = (y oper 1)*w + x/8; \
	mask = 0x01<<(x%8); \
	if((y oper 1) < img->height && !(visited[arrpos] & mask)) { \
		visited[arrpos] |= mask; \
		LLPos2Add(p, Pos2(x, y oper 1)); \
	} \
}

void
fill(Image *img, Pos2 pos, const Color c)
{
	/* the color of the area we have to change the color of */
	Color init;
	/* these are for holding precalculated values */
	Uint x, y, w, arrpos;
	uint8_t mask;
	/* we store what we have already added to the queue in visited */
	uint8_t *visited;

	init = img->getPixel(pos.x, pos.y);
	if(init == c) return;

	w = (img->width+7)/8;
	visited = (uint8_t *) malloc(img->height*w*sizeof(uint8_t));
	for(y=0; y<img->height; y++)
		for(x=0; x<w-1; x++)
			visited[y*w+x] = 0;
	for(y=0; y<img->height; y++)
		visited[y*w + w-1] = 0xff>>((img->width+7)%8+1);

	LLPos2 *p = LLPos2New();
	LLPos2Add(p, pos);
	visited[pos.y*w+pos.x/8] |= 0x01<<(pos.x%8);
	while(1) {
		x = p->f->p.x;
		y = p->f->p.y;
		if(!popLLPos2(p)) return;
		if(img->getPixel(x, y) != init) continue;
		img->setPixel(x, y, c);
		ADDTOQUEUEX(+);
		ADDTOQUEUEX(-);
		ADDTOQUEUEY(+);
		ADDTOQUEUEY(-);
	}
	LLPos2Destroy(p);
	free(visited);
}

#ifndef RELEASE
void
LLPos2PrintLength(LLPos2 *p)
{
	int i=0;
	LPos2 *lp;
	for(lp = p->f; lp != p->l; lp = lp->n) i++;
	printf("Length %d, Head %x, Tail %x\n", i, p->f, p->l);
}

void
LLPos2Print(LLPos2 *p)
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
