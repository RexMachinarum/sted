#include "../lib/sted/terminal.h"
#include "../lib/sted/log.h"

int main(void) {
	stedSetupLogging();
	stedSetupTerminalDrawing();
	stedSetupTerminalInput();

	/* TODO */

	stedCleanupTerminalInput();
	stedCleanupTerminalDrawing();
	stedCleanupLogging();
	return 0;
}
