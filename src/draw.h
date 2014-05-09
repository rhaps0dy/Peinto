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
void drawCircle(Image *img, Pos2 p, Uint r, const Color c, Uint limx, Uint limy);

/* linked position */
typedef struct _LPos2 LPos2;
struct _LPos2 {
	Pos2 p;
	/* next */
	_LPos2 *n;
};

/* polyline which is a linked list without phantom node */
typedef struct _Polyline Polyline;
struct _Polyline {
	/* first */
	LPos2 *f;
	/* last */
	LPos2 *l;
};

Polyline *newPolyline(Pos2 pos);
void destroyPolyline(Polyline *p);
void drawPolyline(Polyline *p, Image *img, const Color c);
void addPosPolyline(Polyline *p, Pos2 pos);
