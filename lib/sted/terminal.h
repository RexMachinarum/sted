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
extern int* stedGetTerminalInput      (size_t* const len_ref, const int wait_for_input);

/*
 * These constants are helper values that are often returned from stedGetTerminalInput().
 * You should only assume that the alpha-numeric characters line up with their
 * ASCII counterparts. 
 */

/* TODO: add support for F1-12 keys ;) */
#define STED_TERMCHAR_CTRL_A    (1)
#define STED_TERMCHAR_CTRL_B    (2)
#define STED_TERMCHAR_CTRL_C    (3)
#define STED_TERMCHAR_CTRL_D    (4)
#define STED_TERMCHAR_CTRL_E    (5)
#define STED_TERMCHAR_CTRL_F    (6)
#define STED_TERMCHAR_CTRL_G    (7)
#define STED_TERMCHAR_CTRL_H    (8)
#define STED_TERMCHAR_CTRL_I    (9)
#define STED_TERMCHAR_CTRL_J    (13)
#define STED_TERMCHAR_CTRL_K    (11)
#define STED_TERMCHAR_CTRL_L    (12)
#define STED_TERMCHAR_CTRL_M    (10)
#define STED_TERMCHAR_CTRL_N    (14)
#define STED_TERMCHAR_CTRL_O    (15)
#define STED_TERMCHAR_CTRL_P    (16)
#define STED_TERMCHAR_CTRL_Q    (17)
#define STED_TERMCHAR_CTRL_R    (18)
#define STED_TERMCHAR_CTRL_S    (19)
#define STED_TERMCHAR_CTRL_T    (20)
#define STED_TERMCHAR_CTRL_U    (21)
#define STED_TERMCHAR_CTRL_V    (22)
#define STED_TERMCHAR_CTRL_W    (23)
#define STED_TERMCHAR_CTRL_X    (24)
#define STED_TERMCHAR_CTRL_Y    (25)
#define STED_TERMCHAR_CTRL_Z    (26)
#define STED_TERMCHAR_ESCAPE    (27)
#define STED_TERMCHAR_BACKSPACE (8)
#define STED_TERMCHAR_TAB       (9)
#define STED_TERMCHAR_SPACE     (32)
#define STED_TERMCHAR_DELETE    (127)

#endif
