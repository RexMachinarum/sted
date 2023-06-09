#ifndef EDITOR_TERMINAL
#define EDITOR_TERMINAL

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

#endif
