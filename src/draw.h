/*requires
"platform.h"
"framework.h"
"image.h"
*/

void drawLine(Image *img, Pos2 p1, Pos2 p2, const Color c);
void drawCircle(Image *img, Pos2 p, Uint r, const Color c);
void fill(Image *img, Pos2 pos, const Color c);

/* linked position */
typedef struct _LPos2 LPos2;
struct _LPos2 {
	Pos2 p;
	/* next */
	LPos2 *n;
};

/* Linked list (with a ghost node) of Pos2 */
typedef struct _LLPos2 LLPos2;
struct _LLPos2 {
	/* first */
	LPos2 *f;
	/* last */
	LPos2 *l;
};

LPos2 *LPos2New(void);
LLPos2 *LLPos2New(void);
void LLPos2Destroy(LLPos2 *p);
void LLPos2Add(LLPos2 *p, Pos2 pos);
void LLPos2Draw(LLPos2 *p, Image *img, const Color c);

/* returns -1 if it popped something, 0 if it didn't */
char popLLPos2(LLPos2 *p);

#ifndef RELEASE
void LLPos2PrintLength(LLPos2 *p);
void LLPos2Print(LLPos2 *p);
#endif
