#ifndef EDITOR_FILE
#define EDITOR_FILE

struct stedCharListNode {
	struct stedCharListNode *next, *prev;
	char value;
};

enum stedCharListOffset {
	stedCharListOffset_Current,
	stedCharListOffset_Next,
	stedCharListOffset_Previous
};

extern struct stedCharListNode* stedFindStartOfCharList  (struct stedCharListNode* list);
extern void                     stedDestroyCharList      (struct stedCharListNode* list);
extern void                     stedAddToCharList        (struct stedCharListNode* list, const char to_add, enum stedCharListOffset offset);
extern void                     stedRemoveFromCharList   (struct stedCharListNode* list);
extern struct stedCharListNode* stedMakeCharListFromFile (const char* const filename);
extern void                     stedSaveCharListToFile   (const struct stedCharListNode* list, const char* const filename);

#endif
