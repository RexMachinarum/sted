#ifndef EDITOR_STRING
#define EDITOR_STRING

#include <stdlib.h>

/* treat stedString and struct stedString* as opaque structs */
struct stedString { 
	struct stedString *next, *prev;
	char value;
};

enum stedStringOffset {
	stedStringOffset_Current,
	stedStringOffset_Next,
	stedStringOffset_Previous
};

extern struct stedString* stedFindStartOfString    (struct stedString* list);
extern void               stedDestroyString        (struct stedString* list);
extern struct stedString* stedCreateString         (void);
extern void               stedAddToString          (struct stedString* list, const char to_add, enum stedStringOffset offset);
extern void               stedRemoveFromString     (struct stedString* list);
extern struct stedString* stedMakeStringFromFile   (const char* const filename);
extern void               stedSaveStringToFile     (const struct stedString* list, const char* const filename);
extern struct stedString* stedGetString            (struct stedString* list, size_t index);
extern char               stedGetStringValue       (struct stedString* list, size_t index);
extern size_t             stedGetStringLength      (struct stedString* list);

#endif
