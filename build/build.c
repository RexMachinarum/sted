/*
 * Compile this file alongside lib/sbbs/sbbs.c using any C89 compatible 
 * compiler to produce a singular executable. This will produce a program
 * that will properly compile the entire project (or at least try to do
 * so). An example of a GCC command building this program is shown below:
 *
 * gcc -ansi -pedantic build.c ../lib/sbbs/sbbs.c -o build
 */

#include <stdlib.h>
#include "../lib/sbbs/sbbs.h"

int main(void) {
	struct sbbsModule* main_module = NULL;

	main_module = sbbsCreateModule("main_module");
	sbbsAddModuleSource(main_module, "../src/main.c", NULL);

	sbbsCompileModule(main_module);
	sbbsCreateExecutable(main_module, "editor", NULL);

	return 0;
}
