#ifndef EDITOR_TERMINAL
#define EDITOR_TERMINAL

/* all of this stuff is for drawing */

typedef unsigned short stedTermDimension;

enum stedTerminalColor {
	stedTerminalColor_Black = 0,
	stedTerminalColor_Red = 1,
	stedTerminalColor_Green = 2,
	stedTerminalColor_Yellow = 3,
	stedTerminalColor_Blue = 4,
	stedTerminalColor_Magenta = 5,
	stedTerminalColor_Cyan = 6,
	stedTerminalColor_White = 7,
	stedTerminalColor_Default = 8
};

enum stedTerminalStyle {
	stedTerminalStyle_Normal = 0,
	stedTerminalStyle_Italic = 1,
	stedTerminalStyle_Bold = 3,
	stedTerminalStyle_BoldItalic
};

extern void              stedSetupTerminalDrawing   (void);
extern void              stedCleanupTerminalDrawing (void);
extern stedTermDimension stedGetTerminalWidth       (void);
extern stedTermDimension stedGetTerminalHeight      (void);
extern void              stedClearTerminal          (void);
extern void              stedDrawTerminal           (void);
extern void              stedDrawCharacter          (stedTermDimension x, stedTermDimension y, const char to_draw, enum stedTerminalColor foreground_color, enum stedTerminalColor background_color, enum stedTerminalStyle style);
extern void              stedDrawString             (stedTermDimension x, stedTermDimension y, const char* to_draw, enum stedTerminalColor foreground_color, enum stedTerminalColor background_color, enum stedTerminalStyle style);

/* All the stuff below is for input processing */

#include <stdlib.h>

extern void             stedSetupTerminalInput    (void);
extern void             stedCleanupTerminalInput  (void);
/*
 * returns an array of character which represent the input that was recieved from the user.
 * 'len_ref' will be set to the length of this array.
 * You MUST free this memory.
*/
extern const int* const stedGetTerminalInput      (size_t* const len_ref); 

#endif
