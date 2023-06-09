
/*
 * Compile this file alongside lib/sbbs/sbbs.c using any C89 compatible 
 * compiler to produce a singular executable. This will produce a program
 * that will properly compile the entire project (or at least try to do
 * so). An example of a GCC command building this program is shown below:
 *
 * gcc -ansi -pedantic build.c ../lib/sbbs/sbbs.c -o build
 */

#include <stdio.h>
#include <stdlib.h>
#include "../lib/sbbs/sbbs.h"

extern size_t              stedGetNullTerminatedModuleListLength (struct sbbsModule** list); 
extern struct sbbsModule** stedAddModuleToNullTerminatedList     (struct sbbsModule** list, struct sbbsModule* module); /* returns a pointer to the start of the list */

int main(int argc, char* argv[]) {

	size_t i = 0;
	struct sbbsModule** os_dependent_modules = NULL;
	struct sbbsModule* main_module = NULL;
	struct sbbsModule* log_module = NULL;
	struct sbbsModule* tmp_module = NULL; /* use this any time that you'd like to */

	/* main module */
	main_module = sbbsCreateModule("main");
	sbbsAddModuleSource(main_module, "../src/main.c", NULL);

	/* logging module */
	log_module = sbbsCreateModule("log");
	sbbsAddModuleSource(log_module, "../src/log.c", NULL);
	sbbsAddModuleDependency(main_module, log_module);

	/* optional modules */
	#ifdef __GLIBC__
		tmp_module = sbbsCreateModule("glibc-terminal");
		sbbsAddModuleSource(tmp_module, "../src/linux/terminal-input.c", NULL);
		sbbsAddModuleSource(tmp_module, "../src/linux/terminal-output.c", NULL);
		os_dependent_modules = stedAddModuleToNullTerminatedList(os_dependent_modules, tmp_module);
	#else
		#error "This platform is not supported by Sted. Please get in touch with the devs to see how long or if this platform will be supported"
	#endif

	/* make os dependent modules dependencies of the main module */
	for (i = 0; os_dependent_modules[i] != NULL; i++) {
		sbbsAddModuleDependency(main_module, os_dependent_modules[i]);
	}

	sbbsCompileModule(main_module);
	sbbsCreateExecutable(main_module, "sted", NULL);

	sbbsDestroyModule(main_module);
	free(os_dependent_modules);

	return 0;
}

size_t stedGetNullTerminatedModuleListLength(struct sbbsModule** list) {
	size_t length = 0, i = 0;
	for (i = 0; list[i] != NULL; i++) {
		length++;
	}
	return length;
}

struct sbbsModule** stedAddModuleToNullTerminatedList(struct sbbsModule** list, struct sbbsModule* module) {
	if (list == NULL) {
		list = calloc(2, sizeof(struct sbbsModule*));
		list[0] = module;
		list[1] = NULL;
	} else {
		const size_t LIST_LEN = stedGetNullTerminatedModuleListLength(list) + 2;
		list = realloc(list, LIST_LEN * sizeof(struct sbbsModule*));
	}
	return list;
}
