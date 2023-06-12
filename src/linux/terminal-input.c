#include "../../lib/sted/terminal.h"

#include <stdio.h>
#include <assert.h>

#ifndef __GLIBC__
	#error "The functions defined in this file only operate on operating systems which have GLibC"
#endif

#include <termios.h>
#include <unistd.h>

static struct termios original_termios;

void stedSetupTerminalInput(void) {
	struct termios new_termios;

	/* ensure that this is a terminal we are messing with */
	assert(isatty(fileno(stdout)));
	assert(tcgetattr(fileno(stdout), &original_termios) == 0);

	/* change to the new termios */
	new_termios.c_lflag = original_termios.c_lflag & ~(ICANON|ECHO|NOFLSH|ISIG);
	new_termios.c_cc[VMIN] = 0;
	new_termios.c_cc[VTIME] = 0;
	assert(tcsetattr(fileno(stdout), TCSADRAIN, &new_termios) == 0);
}

void stedCleanupTerminalInput (void) {
	assert(tcsetattr(fileno(stdout), TCSAFLUSH, &original_termios) == 0);
}

int* stedGetTerminalInput(size_t* const len_ref, const int wait_for_input) {
	/* TODO: if the ESCAPE character is read in, it will wait a tiny bit before processing the input because it wants to know if the user is trying to type some kind of escape character. Fix this maybe? ;) No one else does this but it would be nice to be the first ;)))) */
	int raw_input = '\0';
	int* input = NULL;
	size_t raw_input_capacity = 1, raw_input_elements = 0;

	/* take input */
	input = calloc(raw_input_capacity, sizeof(int));
do_input:
	while (read(fileno(stdin), &raw_input, sizeof(int))) {
		if (raw_input_elements + 1 > raw_input_capacity) {
			raw_input_capacity *= 2;
			input = realloc(input, sizeof(int) * raw_input_capacity);
		}
		/* TODO: do the weird tree handling shit for F1-12 ;) */
		input[raw_input_elements++] = raw_input;
	}
	*len_ref = raw_input_elements;
	if (wait_for_input && *len_ref < 1) {
		goto do_input;
	}

	return input;
}
