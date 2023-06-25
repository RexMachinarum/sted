#include "../../lib/sted/terminal.h"

#include <stdio.h>
#include <assert.h>

#ifndef __GLIBC__
	#error "The functions defined in this file only operate on operating systems which have GLibC"
#else

#include <unistd.h>
#include <sys/ioctl.h>

/* static functions */

static int IsTTY() {
	return isatty(fileno(stdout));
}

/* exported functions */

void stedSetupTerminalDrawing(void) {
	/* causes printing to only work when stdout is flushed */
	setvbuf(stdout, NULL, _IOFBF, 0);
	fputs("\e[2J\e[?1049h\e[?25l", stdout); /* clears the screen, enables alternate terminal buffer & makes cursor invisible */
	fflush(stdout);
}

void stedCleanupTerminalDrawing(void) {
	/* disbles alternate terminal buffer, makes cursor visible, and clears the screen */
	fputs("\e[?1049l\e[?25h\e[2J", stdout);
	fflush(stdout);
	setvbuf(stdout, NULL, _IONBF, BUFSIZ); 	/* returns to standard bufferless terminal drawing */
}

stedTermDimension stedGetTerminalWidth(void) {
	assert(IsTTY());
	struct winsize term_size;
	ioctl(fileno(stdout), TIOCGWINSZ, &term_size);
	return term_size.ws_col;
}

stedTermDimension stedGetTerminalHeight(void) {
	assert(IsTTY());
	struct winsize term_size;
	ioctl(fileno(stdout), TIOCGWINSZ, &term_size);
	return term_size.ws_row;
}

void stedClearTerminal(void) {
	fprintf(stdout, "\e[2J");
}

void stedDrawTerminal(void) {
	fflush(stdout);
}

/* TODO: add a draw pass system that allows the previously rendered frame 
 * against the current frame that will be produced and only render the 
 * differences between these two frames. This saves a lot of performance
 * but the code is janky to implement (because it needs persistent state
 * so I'll do that later)*/

void stedMoveTerminalCursorTo       (stedTermDimension pos_x, stedTermDimension pos_y) {
	fprintf(stdout, "\e[%d;%dH", pos_y, pos_x);
}

void stedSetTerminalStyle           (enum stedTerminalStyle style) {
	if (style == stedTerminalStyle_BoldItalic) {
		fputs("\e[1;3m]", stdout);
	} else {
		fprintf(stdout, "\e[%dm", style);
	}
}

void stedSetTerminalBackgroundColor (enum stedTerminalColor color) {
	fprintf(stdout, "\e[%dm", 40 + color);
}

void stedSetTerminalForegroundColor (enum stedTerminalColor color) {
	fprintf(stdout, "\e[%dm", 30 + color);
}

void stedDrawCharToTerminal(char c) {
	fputc(c, stdout);
}

void stedDrawCStringToTerminal(char* str) {
	fputs(str, stdout);
}


#endif
