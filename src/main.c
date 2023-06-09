#include "../lib/sted/terminal.h"
#include <unistd.h>

int main(void) {
	stedSetupTerminalDrawing();
	stedSetupTerminalInput();

	stedCleanupTerminalInput();
	stedCleanupTerminalDrawing();
	return 0;
}
