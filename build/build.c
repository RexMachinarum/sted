
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

enum stedBuildMode {
	stedBuildMode_Release,
	stedBuildMode_Debug
};

int main(int argc, char* argv[]) {

	size_t i = 0;
	struct sbbsModule
		*main_module = NULL,
		*general_module = NULL,
		*terminal_module = NULL,
		*tmp_module = NULL;
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
		const char* RELEASE_ARGS = "-std=c89 -O2";
		const char* DEBUG_ARGS = "-ggdb";
		size_t cl_args_len = strlen(RELEASE_ARGS) + 1;
		if (build_mode == stedBuildMode_Debug) {
			cl_args_len += strlen(DEBUG_ARGS) + 1;
		}
		universal_cl_args = calloc(cl_args_len + 1, sizeof(char));
		strcat(universal_cl_args, RELEASE_ARGS);
		strcat(universal_cl_args, " ");
		if (build_mode == stedBuildMode_Debug) {
			strcat(universal_cl_args, DEBUG_ARGS);
		}
	}

	/* main module */
	main_module = sbbsCreateModule("main");
	sbbsAddModuleSource(main_module, "../src/main.c", universal_cl_args);

	/* general module; used in every build type */
	general_module = sbbsCreateModule("general");
	sbbsAddModuleSource(general_module, "../src/log.c", universal_cl_args);
	sbbsAddModuleSource(general_module, "../src/string.c", universal_cl_args);
	sbbsAddModuleDependency(main_module, general_module);

	/* optional terminal module; right now this is forced */
	terminal_module = sbbsCreateModule("terminal");
	#ifdef __GLIBC__
		sbbsAddModuleSource(terminal_module, "../src/linux/terminal-input.c", universal_cl_args);
		sbbsAddModuleSource(terminal_module, "../src/linux/terminal-output.c", universal_cl_args);
		sbbsAddModuleDependency(main_module, terminal_module);
	#else
		#error "This platform is not supported by Sted. Please get in touch with the devs to see how long or if this platform will be supported"
	#endif

	/* compile the program */
	sbbsCompileModule(main_module);
	sbbsCreateExecutable(main_module, "sted", universal_cl_args);

	/* cleanup */
	if (universal_cl_args != NULL) {
		free(universal_cl_args);
	}
	sbbsDestroyModule(main_module);

	return 0;
}
