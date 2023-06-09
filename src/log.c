#include "../lib/sted/log.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#ifdef __GLIBC__
	#include <unistd.h>
	#include <pwd.h>
#endif

static FILE* log_file = NULL;

void stedSetupLogging(void) {
	#ifdef __GLIBC__
	{
		struct passwd* user_info;
		user_info = getpwnam(getlogin());
		if (user_info == NULL) {
			log_file = fopen("sted.log", "a+");
			assert(log_file != NULL);
		} else {
			char* xdg_state_log = calloc(strlen(user_info->pw_dir) + strlen("/.local/state/sted.log") + 1, sizeof(char));
			strcat(xdg_state_log, user_info->pw_dir);
			strcat(xdg_state_log, "/.local/state/sted.log");
			log_file = fopen(xdg_state_log, "a+");
			free(xdg_state_log);
		}
	}
	#else
		log_file = fopen("sted.log", "a+");
		assert(log_file != NULL); /* TODO: make this nice ;) */
	#endif
}

void stedCleanupLogging(void) {
	if (log_file != NULL) {
		fclose(log_file);
	}
}

void _stedLog(char* msg, const char* const file_name, const int line_number) {
	if (log_file != NULL) {
		char i = 0;
		char ascii_time[27] = { 0 };
		time_t current_time;
		current_time = time(NULL);
		strcpy(ascii_time, ctime(&current_time));
		for (i = 26; i > 0; i--) {
			if (ascii_time[i] == '\n') {
				ascii_time[i] = '\0';
				break;
			}
		}
		fprintf(log_file, "[%s] %s::%.6d --> %s\n", ascii_time, file_name, line_number, msg);
	}
}

void _stedLogWarning(char* msg, const char* const file_name, const int line_number) {
	if (log_file != NULL) {
		char i = 0;
		char ascii_time[27] = { 0 };
		time_t current_time;
		current_time = time(NULL);
		strcpy(ascii_time, ctime(&current_time));
		for (i = 26; i > 0; i--) {
			if (ascii_time[i] == '\n') {
				ascii_time[i] = '\0';
				break;
			}
		}
		fprintf(log_file, "[%s] %s::%.6d !!WARNING!! --> %s\n", ascii_time, file_name, line_number, msg);
	}
}

void _stedLogError(char* msg, const char* const file_name, const int line_number) {
	if (log_file != NULL) {
		char i = 0;
		char ascii_time[27] = { 0 };
		time_t current_time;
		current_time = time(NULL);
		strcpy(ascii_time, ctime(&current_time));
		for (i = 26; i > 0; i--) {
			if (ascii_time[i] == '\n') {
				ascii_time[i] = '\0';
				break;
			}
		}
		fprintf(log_file, "[%s] %s::%.6d !!!ERROR!!! --> %s\n", ascii_time, file_name, line_number, msg);
	}
}
