#include "../../lib/sted/terminal.h"

#include <stdio.h>
#include <assert.h>

#ifndef __GLIBC__
	#error "The functions defined in this file only operate on operating systems which have GLibC"
#else

#include <unistd.h>
#include <sys/ioctl.h>

static int IsTTY() {
	FILE* f = NULL;
	int status = 0;

	f = fopen(ctermid(NULL), "r");
	status = isatty(fileno(f));
	if (f != NULL) {
		fclose(f);
	}

	return status;
}

void stedSetupTerminalDrawing(void) {
	/* causes printing to only work when stdout is flushed */
	setvbuf(stdout, NULL, _IOFBF, BUFSIZ);
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
	fflush(stdout);
}

void stedDrawTerminal(void) {
	fflush(stdout);
}

void stedDrawCharacter(stedTermDimension x, stedTermDimension y, const char to_draw, enum stedTerminalColor foreground_color, enum stedTerminalColor background_color, enum stedTerminalStyle style) {
	const short ansi_escape_code_foreground_color = foreground_color + 30;
	const short ansi_escape_code_background_color = background_color + 40;
	if (style == stedTerminalStyle_BoldItalic) {
		fputs("\e[1;3m", stdout);
		fprintf(stdout, "\e[%d;%dH\e[%d;%dm%c", y, x, ansi_escape_code_foreground_color, ansi_escape_code_background_color, to_draw);
	} else {
		fprintf(stdout, "\e[%d;%dH\e[%d;%d;%dm%c", y, x, style, ansi_escape_code_foreground_color, ansi_escape_code_background_color, to_draw);
	}
}

void stedDrawString(stedTermDimension x, stedTermDimension y, const char* to_draw, enum stedTerminalColor foreground_color, enum stedTerminalColor background_color, enum stedTerminalStyle style) {
	const short ansi_escape_code_foreground_color = foreground_color + 30;
	const short ansi_escape_code_background_color = background_color + 40;
	if (style == stedTerminalStyle_BoldItalic) {
		fputs("\e[1;3m", stdout);
		fprintf(stdout, "\e[%d;%dH\e[%d;%dm%s", y, x, ansi_escape_code_foreground_color, ansi_escape_code_background_color, to_draw);
	} else {
		fprintf(stdout, "\e[%d;%dH\e[%d;%d;%d;%dm%s", y, x, ansi_escape_code_foreground_color, ansi_escape_code_background_color, ansi_escape_code_foreground_color, ansi_escape_code_background_color, to_draw);
	}
}

#endif
