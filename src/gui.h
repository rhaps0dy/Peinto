/*requires
"platform.h"
"framework.h"
"image.h"
*/

// describes the current state of the GUI
typedef enum {
	LINE,
	CIRCLE,
	POLYGONAL,
	SHAPE,
	FREEHAND,
	BUCKET,
} GUITool;

typedef struct {
	// last position the mouse was pressed in
	Pos2 lastMDown;
	GUITool tool;
	// are we drawing? (true -1/false 0)
	char dwg;
} GUIState;

void showREADME(void);

/* struct that defines a color selector */
typedef struct _ColorSelector ColorSelector;
struct _ColorSelector {
	Color c;
	Uint margins;
	Uint w, h, x, y;
	char visible;
};

void drawColorSelector(ColorSelector *cs, Image *img);
/* returns -1 if properly handled, 0 if not */
char handleClick(ColorSelector *cs, Pos2 p);
