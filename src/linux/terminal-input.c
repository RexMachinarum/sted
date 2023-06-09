#include "../../lib/sted/terminal.h"

#include <stdio.h>
#include <assert.h>

#ifndef __GLIBC__
	#error "The functions defined in this file only operate on operating systems which have GLibC"
#endif

#include <termios.h>

static struct termios original_termios;

void stedSetupTerminalInput(void) {
	FILE* f = NULL;
	struct termios new_termios;

	/* ensure that this is a terminal we are messing with */
	f = fopen(ctermid(NULL), "r");
	assert(f != NULL); /* TODO: this is bad, please change ;) */
	assert(tcgetattr(fileno(f), &original_termios) == 0);

	/* change to the new termios */
	new_termios.c_lflag = original_termios.c_lflag & ~(ICANON|ECHO|NOFLSH|ISIG);
	assert(tcsetattr(fileno(f), TCSADRAIN, &new_termios) == 0);

	/* cleanup */
	fclose(f);
}

void stedCleanupTerminalInput (void) {
	FILE* f = NULL;

	f = fopen(ctermid(NULL), "r");
	assert(f != NULL); /* TODO: this is also bad, please change later ;) */
	assert(tcsetattr(fileno(f), TCSAFLUSH, &original_termios) == 0);
	fclose(f);
}

const int* const stedGetTerminalInput(size_t* const len_ref) {
	*len_ref = 0;
	return NULL;
}
