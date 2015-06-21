#ifndef GRAPHICS_CONSTANT_H
#define GRAPHICS_CONSTANT_H


#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif
#define LOG_TAG "Main"

#define LOG_TAGQI    __FILE__ ":" STR(__LINE__)
#define STR(x) STRINGIFY(x)
#define STRINGIFY(x) # x

#define MAX_CHAR 64
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <GL/glew.h>

#include <core/glm.hpp>
#include <core/gtc/matrix_transform.hpp>
#include <core/glm.hpp>
#include <core/gtx/quaternion_gtx.hpp>
#include <core/gtc/quaternion.hpp>

#ifdef _WIN32
#define LOGI printf
#endif

//! The depth of the projection matrix stack.
#define MAX_PROJECTION_MATRIX	2

//! The depth of the model matrix stack.
#define MAX_MODEL_MATRIX	20

//! The depth of the view matrix stack.
#define MAX_VIEW_MATRIX	8

//! The depth of the texture matrix stack.
#define MAX_TEXTURE_MATRIX		2

#ifdef _WIN32
#else
#define MAX_PATH	256
#endif

#define DEG_TO_RAD	M_PI / 180.0f

#define RAD_TO_DEG	90.0f / M_PI

#define BUFFER_OFFSET( x ) ( ( char * )NULL + x )

#define CLAMP( x, low, high ) ( ( x > high ) ? high : ( ( x < low ) ? low : x ) )

//! Maximum number of program possible.
#define MAX_PROGRAM_SIZE 50

enum
{
	
    //! The model matrix identifier.
	MODEL_MATRIX = 0,

	//! The view matrix identifier.
	VIEW_MATRIX = 1,

	//! The projection matrix identifier.
	PROJECTION_MATRIX = 2,

	//! The texture matrix identifier.
	TEXTURE_MATRIX	  = 3
};

enum ModelType{
	//! The Triangle Model identifier.
	TriangleType = 0,
	CubeType = 1,
	TeapotType = 2,
	ObjFileType = 3,
	ThreeDSFileType = 4,
	TransformFeedbackType = 5,
	GridType = 6,
	ImageDemoType = 7,
    FontEnglish = 8,
    FontPunjabi = 9,
    FontThai = 10,
    FontArabic = 11,
    FontTamil = 12,
    BUTTON = 13,
    PIXMAP = 14,
    RECTANGLE = 15,
	MESH = 16,
	None = 17,
};

// Language Type for Localizaion support
enum LanguageType
{
    English = 0,
    Arabic  = 1,
    Thai    = 2,
    Punjabi = 3,
    Tamil = 4,
};

enum CameraType{
	//! The Ortho Camera type identifier.
	ortho = 0,

	//! The Perespective Camera type identifier.
	perespective
};

enum TextureTypeEnum{
    INVALID_DIMENSIONAL_TEXTURE,
    ONE_DIMENSIONAL_TEXTURE,
    TWO_DIMENSIONAL_TEXTURE,
    THREE_DIMENSIONAL_TEXTURE,
    TWO_DIMENSIONAL_ARRAY_TEXTURE,
    CUBE_MAP_TEXTURE
};

enum ImageTypeEnum{
    BITMAPS,
    PNG,
    JPEG,
    NO_IMAGE
};

#endif
