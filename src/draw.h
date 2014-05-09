/*requires
"platform.h"
"framework.h"
"image.h"
*/

/* struct that defines a color selector */
/*typedef struct {
	Color c;
	Uint margins;
	Uint w, h, x, y;
} */

void drawLine(Image *img, Pos2 p1, Pos2 p2, const Color c);
void drawCircle(Image *img, Pos2 p, Uint r, const Color c);
void fill(Image *img, Pos2 pos, const Color c);

/* linked position */
typedef struct _LPos2 LPos2;
struct _LPos2 {
	Pos2 p;
	/* next */
	_LPos2 *n;
};

/* polyline which is a linked list with phantom node */
typedef struct _Polyline Polyline;
struct _Polyline {
	/* first */
	LPos2 *f;
	/* last */
	LPos2 *l;
};

LPos2 *newLPos2(void);
Polyline *newPolyline(void);
void destroyPolyline(Polyline *p);
void addPosPolyline(Polyline *p, Pos2 pos);
void drawPolyline(Polyline *p, Image *img, const Color c);

/* returns -1 if it popped something, 0 if it didn't */
char popPolyline(Polyline *p);

#ifndef RELEASE
void printPolylineLength(Polyline *p);
void printPolyline(Polyline *p);
#endif
