#include <stdio.h>
#include <string.h>
#include "../lib/sted/terminal.h"
#include "../lib/sted/log.h"
#include "../lib/sted/types.h"
#include "../lib/sted/string.h"

extern void usage(const char* const prog_name);

static bool fullscreen_check(struct stedString* string, size_t index, void* opt_data) {
	if (stedGetStringValue(string, index) == '#') {
		return true;
	}
	return true;
}

static size_t fullscreen_draw(struct stedString* string, size_t index, void* opt_data) {
	fputc('e', stdout);
	return index + 1;
}

int main(int argc, char* argv[]) {
	size_t i = 0;
	bool quit_early = false;
	struct stedString* string = NULL;

	/* initial setup */
	stedSetupLogging();

	/* process arguments */
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			usage(argv[0]);
			quit_early = true;
			break;
		} else { /* assume it's a file */
			string = stedMakeStringFromFile(argv[i]);
			break;
		}
	}

	/* start the editor */
	if (!quit_early) {
		bool running = true;
		struct {
			int* data;
			size_t length;
		} input;
		struct stedRect terminal_screen;
		terminal_screen.x = 0;
		terminal_screen.y = 0;
		size_t cursor = 0; /* index into the string */

		/* initialize terminal */
		stedSetupTerminalInput();
		stedSetupTerminalDrawing();

		while (running) {
			/* logic */
			terminal_screen.width = stedGetTerminalWidth();
			terminal_screen.height = stedGetTerminalHeight();

			/* drawing */
			stedClearTerminal();

			/* drawing the file */
			if (string != NULL) {
				stedTermDimension cur_x = terminal_screen.x + 1;
				stedTermDimension cur_y = terminal_screen.y;
				for (i = 0; i < stedGetStringLength(string); i++) {
					const char CURRENT_CHAR = stedGetStringValue(string, i);
					if (i == cursor) {
						stedSetTerminalForegroundColor(stedTerminalColor_Black);
						stedSetTerminalBackgroundColor(stedTerminalColor_White);
					} else {
						stedSetTerminalForegroundColor(stedTerminalColor_White);
						stedSetTerminalBackgroundColor(stedTerminalColor_Black);
					}
					if (cur_x - terminal_screen.x > terminal_screen.width) {
						cur_x = terminal_screen.x + 1;
						cur_y += 1;
					}
					if (cur_y - terminal_screen.y > terminal_screen.height) {
						break;
					}
					if (CURRENT_CHAR == '\n') {
						cur_x = terminal_screen.x + 1;
						cur_y += 1;
					} else if (CURRENT_CHAR == '\t') {
						cur_x += 4;
						fputs("    ", stdout);
					} else {
						fprintf(stdout, "\e[%d;%dH", cur_y, cur_x);
						fputc(CURRENT_CHAR, stdout);
						cur_x += 1;
					}
				}
			}

			/* display frame */
			stedDrawTerminal();

			/* input */
			input.data = stedGetTerminalInput(&(input.length), true);
			for (i = 0; i < input.length; i++) {
				if (input.data[i] == STED_TERMCHAR_CTRL_C) {
					running = false;
					break;
				}
				if (input.data[i] == STED_TERMCHAR_CTRL_N && cursor <= stedGetStringLength(string)) {
					cursor++;
				}
				if (input.data[i] == STED_TERMCHAR_CTRL_P && cursor > 0) {
					cursor--;
				}
			}

			/* cleanup */
			free(input.data);
		}

		stedCleanupTerminalInput();
		stedCleanupTerminalDrawing();
	}

	/* cleanup */
	if (string != NULL) {
		stedDestroyString(string);
	}

	stedCleanupLogging();
	return 0;
}

void usage(const char* const prog_name) {
	const char* const ARG_FORMAT = "  %-20s%-40s\n";
	printf("Usage: %s [options] file\n", prog_name);
	puts("Options:");
	printf(ARG_FORMAT, "-h, --help", "shows this help dialogue");
}
