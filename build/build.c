
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
#include <string.h>
#include "../lib/sbbs/sbbs.h"

extern size_t              stedGetNullTerminatedModuleListLength (struct sbbsModule** list); 
extern struct sbbsModule** stedAddModuleToNullTerminatedList     (struct sbbsModule** list, struct sbbsModule* module); /* returns a pointer to the start of the list */

enum stedBuildMode {
	stedBuildMode_Release,
	stedBuildMode_Debug
};

int main(int argc, char* argv[]) {

	size_t i = 0;
	struct sbbsModule** os_dependent_modules = NULL;
	struct sbbsModule* main_module = NULL;
	struct sbbsModule* log_module = NULL;
	struct sbbsModule* file_module = NULL;
	struct sbbsModule* tmp_module = NULL; /* use this any time that you'd like to */
	char* universal_cl_args = NULL;
	enum stedBuildMode build_mode;

	/* build modes */
	if (argc == 1) {
		printf("%s (Release|Debug)", argv[0]);
		return 1;
	} else {
		for (i = 1; i < argc; i++) {
			if (strcmp(argv[i], "Release") == 0) {
				build_mode = stedBuildMode_Release;
			}
			if (strcmp(argv[i], "Debug") == 0) {
				build_mode = stedBuildMode_Debug;
			}
		}
	}

	/* force C89 compatibility if using gcc */
	if (sbbsWhichCompiler() == sbbsCompilerType_gcc) {
		const char* release_args = "-std=c89 -O2";
		const char* debug_args = "-ggdb";
		size_t cl_args_len = strlen(release_args) + 1;
		if (build_mode == stedBuildMode_Debug) {
			cl_args_len += strlen(debug_args) + 1;
		}
		universal_cl_args = calloc(cl_args_len + 1, sizeof(char));
		strcat(universal_cl_args, release_args);
		strcat(universal_cl_args, " ");
		if (build_mode == stedBuildMode_Debug) {
			strcat(universal_cl_args, debug_args);
		}
	}

	/* main module */
	main_module = sbbsCreateModule("main");
	sbbsAddModuleSource(main_module, "../src/main.c", universal_cl_args);

	/* logging module */
	log_module = sbbsCreateModule("log");
	sbbsAddModuleSource(log_module, "../src/log.c", universal_cl_args);
	sbbsAddModuleDependency(main_module, log_module);

	/* file module */
	file_module = sbbsCreateModule("file");
	sbbsAddModuleSource(file_module, "../src/file.c", universal_cl_args);
	sbbsAddModuleDependency(main_module, file_module);

	/* optional modules */
	#ifdef __GLIBC__
		tmp_module = sbbsCreateModule("glibc-terminal");
		sbbsAddModuleSource(tmp_module, "../src/linux/terminal-input.c", universal_cl_args);
		sbbsAddModuleSource(tmp_module, "../src/linux/terminal-output.c", universal_cl_args);
		os_dependent_modules = stedAddModuleToNullTerminatedList(os_dependent_modules, tmp_module);
	#else
		#error "This platform is not supported by Sted. Please get in touch with the devs to see how long or if this platform will be supported"
	#endif

	/* make os dependent modules dependencies of the main module */
	for (i = 0; os_dependent_modules[i] != NULL; i++) {
		sbbsAddModuleDependency(main_module, os_dependent_modules[i]);
	}

	/* compile the program */
	sbbsCompileModule(main_module);
	sbbsCreateExecutable(main_module, "sted", universal_cl_args);

	/* cleanup */
	if (universal_cl_args != NULL) {
		free(universal_cl_args);
	}
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
