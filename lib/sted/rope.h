#ifndef EDITOR_ROPE
#define EDITOR_ROPE

#include <stdlib.h>
#include "types.h"

struct stedRope {
	enum stedTreeNodeType type;
	union {
		struct {
			char* data_;
			char elements_used_;
		} leaf_;
		struct {
			struct stedRope *left, *right;
			size_t len_of_children;
		} branch_;
	};
};

#endif
