#include "../lib/sted/string.h"
#include "../lib/sted/log.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct stedString* stedFindStartOfCharList(struct stedString* string) {
	struct stedString* iter = string;
	while (iter->prev != NULL) {
		iter = iter->prev;
	}
	return iter;
}

void stedDestroyString(struct stedString* string) {
	struct stedString* iter = string;
	while (iter != NULL) {
		struct stedString* prev = iter;
		iter = iter->next;
		free(prev);
	}
}

struct stedString* stedCreateString(void) {
	struct stedString* new = malloc(sizeof(struct stedString));
	new->next = NULL;
	new->prev = NULL;
	new->value = '\0';
	return new;
}

void stedAddToString(struct stedString* string, const char to_add, enum stedStringOffset offset) {
	assert(offset != stedStringOffset_Current);
	if (offset == stedStringOffset_Next) {
		if (string->next == NULL && string->value == '\0') {
			string->value = to_add;
		} else if (string->next == NULL) {
			string->next = malloc(sizeof(struct stedString));
			string->next->next = NULL;
			string->next->prev = string;
			string->next->value = to_add;
		} else {
			struct stedString* old_next = string->next;
			string->next = malloc(sizeof(struct stedString));
			string->next->next = old_next;
			string->next->next->prev = string->next;
			string->next->value = to_add;
			string->next->prev = string;
		}
	} else if (offset == stedStringOffset_Previous) {
		if (string->prev == NULL && string->value == '\0') {
			string->value = to_add;
		} else if (string->prev == NULL) {
			string->prev = malloc(sizeof(struct stedString));
			string->prev->prev = NULL;
			string->prev->next = string;
			string->prev->value = to_add;
		} else {
			struct stedString* old_prev = string->prev;
			string->prev = malloc(sizeof(struct stedString));
			string->prev->prev = old_prev;
			string->prev->next = string;
			string->prev->prev->next = string->prev;
			string->prev->value = to_add;
		}
	}
}

void stedRemoveFromString(struct stedString* string) {
	string->prev->next = string->next;
	string->next->prev = string->prev;
	free(string);
}

struct stedString* stedMakeStringFromFile(const char* const filename) {
	FILE* file = NULL;
	struct stedString* new_list = NULL, *iter = NULL;
	int ch;

	file = fopen(filename, "r");
	if (file == NULL) {
		return NULL;
	}

	while ((ch = fgetc(file)) != EOF) {
		if (new_list == NULL) {
			new_list = malloc(sizeof(struct stedString));
			iter = new_list;
			iter->next = NULL;
			iter->prev = NULL;
			iter->value = (char)ch;
		} else {
			iter->next = malloc(sizeof(struct stedString));
			iter->next->value = (char)ch;
			iter->next->next = NULL;
			iter->next->prev = iter;
			iter = iter->next;
		}
	}

	fclose(file);

	return new_list;
}

void stedSaveStringToFile(const struct stedString* string, const char* const filename) {
	const struct stedString* iter = string;

	FILE* file = fopen(filename, "w");
	assert(file != NULL);
	for (iter = string; iter != NULL; iter = iter->next) {
		fputc(iter->value, file);
	}
	fclose(file);
}

struct stedString* stedGetString(struct stedString* string, size_t index) {
	size_t i = 0;
	struct stedString* iter = string;
	for (i = 0; i < index; i++) {
		if (iter == NULL) {
			return NULL;
		} else {
			iter = iter->next;
		}
	}
	return iter;
}

char stedGetStringValue(struct stedString* string, size_t index) {
	return stedGetString(string, index)->value;
}

size_t stedGetStringLength(struct stedString* string) {
	size_t length = 0;
	struct stedString* iter = string;
	while (iter != NULL) {
		iter = iter->next;
		length++;
	}
	return length;
}
