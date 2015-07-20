#ifndef SCHEMES_h
#define SCHEMES_h
#include "glutils.h"

enum BufferScheme{
	BUFFER_VA = 0,
	BUFFER_VBO = 1,
	BUFFER_VAO = 2,
};

enum DrawingScheme{
	DRAW_ARRAY = 0,
	DRAW_ELEMENT = 1,
};

enum PrimitiveScheme{
	PRIMITIVE_POINTS = 0,
	PRIMITIVE_LINES = 1,
	PRIMITIVE_LINE_LOOP = 2,
	PRIMITIVE_LINE_STRIP = 3,
	PRIMITIVE_TRIANGLES = 4,
	PRIMITIVE_TRIANGLE_STRIP = 5,
	PRIMITIVE_TRIANGLE_FAN = 6,
};

#endif
