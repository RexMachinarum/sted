#ifndef EDITOR_TYPES
#define EDITOR_TYPES

#ifndef bool
#define bool unsigned char
#define true (1)
#define false (0)
#endif

enum stedTreeNodeType {
	stedTreeNodeType_Leaf,
	stedTreeNodeType_Branch
};

struct stedRect {
	long unsigned int x, y, width, height;
};

#endif
