#include "../lib/sted/file.h"
#include "../lib/sted/log.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct stedCharListNode* stedFindStartOfCharList  (struct stedCharListNode* list) {
	struct stedCharListNode* iter = list;
	while (iter->prev != NULL) {
		iter = iter->prev;
	}
	return iter;
}

void stedDestroyCharList(struct stedCharListNode* list) {
	struct stedCharListNode* iter = list;
	while (iter != NULL) {
		struct stedCharListNode* prev = iter;
		iter = iter->next;
		free(prev);
	}
}

void stedAddToCharList(struct stedCharListNode* list, const char to_add, enum stedCharListOffset offset) {
	assert(offset != stedCharListOffset_Current);
	if (offset == stedCharListOffset_Next) {
		if (list->next == NULL) {
			list->next = malloc(sizeof(struct stedCharListNode));
			list->next->next = NULL;
			list->next->prev = list;
			list->next->value = to_add;
		} else {
			struct stedCharListNode* old_next = list->next;
			list->next = malloc(sizeof(struct stedCharListNode));
			list->next->next = old_next;
			list->next->next->prev = list->next;
			list->next->value = to_add;
			list->next->prev = list;
		}
	} else if (offset == stedCharListOffset_Previous) {
		if (list->prev == NULL) {
			list->prev = malloc(sizeof(struct stedCharListNode));
			list->prev->prev = NULL;
			list->prev->next = list;
			list->prev->value = to_add;
		} else {
			struct stedCharListNode* old_prev = list->prev;
			list->prev = malloc(sizeof(struct stedCharListNode));
			list->prev->prev = old_prev;
			list->prev->next = list;
			list->prev->prev->next = list->prev;
			list->prev->value = to_add;
		}
	}
}

void stedRemoveFromCharList(struct stedCharListNode* list) {
	list->prev->next = list->next;
	list->next->prev = list->prev;
	free(list);
}

struct stedCharListNode* stedMakeCharListFromFile (const char* const filename) {
	FILE* file = NULL;
	struct stedCharListNode* new_list = NULL, *iter = NULL;
	int ch;

	file = fopen(filename, "r");
	if (file == NULL) {
		return NULL;
	}

	while ((ch = fgetc(file)) != EOF) {
		if (new_list == NULL) {
			new_list = malloc(sizeof(struct stedCharListNode));
			iter = new_list;
			iter->next = NULL;
			iter->prev = NULL;
			iter->value = (char)ch;
		} else {
			iter->next = malloc(sizeof(struct stedCharListNode));
			iter->next->value = (char)ch;
			iter->next->next = NULL;
			iter->next->prev = iter;
			iter = iter->next;
		}
	}

	fclose(file);

	return new_list;
}

void stedSaveCharListToFile(const struct stedCharListNode* list, const char* const filename) {
	const struct stedCharListNode* iter = list;

	FILE* file = fopen(filename, "w");
	assert(file != NULL);
	for (iter = list; iter != NULL; iter = iter->next) {
		fputc(iter->value, file);
	}
	fclose(file);
}
