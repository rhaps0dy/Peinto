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
