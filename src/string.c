#include "../lib/sted/string.h"
#include "../lib/sted/log.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct stedString* stedFindStartOfCharList(struct stedString* list) {
	struct stedString* iter = list;
	while (iter->prev != NULL) {
		iter = iter->prev;
	}
	return iter;
}

void stedDestroyString(struct stedString* list) {
	struct stedString* iter = list;
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

void stedAddToString(struct stedString* list, const char to_add, enum stedStringOffset offset) {
	assert(offset != stedStringOffset_Current);
	if (offset == stedStringOffset_Next) {
		if (list->next == NULL && list->value == '\0') {
			list->value = to_add;
		} else if (list->next == NULL) {
			list->next = malloc(sizeof(struct stedString));
			list->next->next = NULL;
			list->next->prev = list;
			list->next->value = to_add;
		} else {
			struct stedString* old_next = list->next;
			list->next = malloc(sizeof(struct stedString));
			list->next->next = old_next;
			list->next->next->prev = list->next;
			list->next->value = to_add;
			list->next->prev = list;
		}
	} else if (offset == stedStringOffset_Previous) {
		if (list->prev == NULL && list->value == '\0') {
			list->value = to_add;
		} else if (list->prev == NULL) {
			list->prev = malloc(sizeof(struct stedString));
			list->prev->prev = NULL;
			list->prev->next = list;
			list->prev->value = to_add;
		} else {
			struct stedString* old_prev = list->prev;
			list->prev = malloc(sizeof(struct stedString));
			list->prev->prev = old_prev;
			list->prev->next = list;
			list->prev->prev->next = list->prev;
			list->prev->value = to_add;
		}
	}
}

void stedRemoveFromString(struct stedString* list) {
	list->prev->next = list->next;
	list->next->prev = list->prev;
	free(list);
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

void stedSaveStringToFile(const struct stedString* list, const char* const filename) {
	const struct stedString* iter = list;

	FILE* file = fopen(filename, "w");
	assert(file != NULL);
	for (iter = list; iter != NULL; iter = iter->next) {
		fputc(iter->value, file);
	}
	fclose(file);
}

struct stedString* stedGetString(struct stedString* list, size_t index) {
	size_t i = 0;
	struct stedString* iter = list;
	for (i = 0; i < index; i++) {
		if (iter == NULL) {
			return NULL;
		} else {
			iter = iter->next;
		}
	}
	return iter;
}

char stedGetStringValue(struct stedString* list, size_t index) {
	return stedGetString(list, index)->value;
}

size_t stedGetStringLength(struct stedString* list) {
	size_t length = 0;
	struct stedString* iter = list;
	while (iter != NULL) {
		iter = iter->next;
		length++;
	}
	return length;
}
