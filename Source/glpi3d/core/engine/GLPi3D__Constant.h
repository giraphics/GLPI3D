#ifndef GRLC_OPENGLES_CONSTANT_H
#define GRLC_OPENGLES_CONSTANT_H

// Include common dependencies
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
//#include <GLES2/gl2.h>
#include <GL/glew.h>

//#include <GLES2/gl2ext.h>
#include <core/glm.hpp>
#include <core/gtc/matrix_transform.hpp>
#include <core/glm.hpp>
#include <core/gtx/quaternion_gtx.hpp>
#include <core/gtc/quaternion.hpp>

/** Unsigned 8-bit datatype : 0..255                                    */
typedef unsigned char           uint8;

/** Signed 8-bit datatype   : -128...+127                               */
typedef signed char              int8;

/** Unsigned 16-bit datatype: 0..65535                                  */
typedef unsigned short int     uint16;

/** Signed 16-bit datatype  : -32768..+32767                            */
typedef signed short int        int16;

/** Unsigned 32-bit datatype: 0..4294967295                             */  
typedef unsigned long int      uint32;

/** Signed 32-bit datatype  : -2147483648..2147483647                   */  
typedef signed long int         int32;

namespace GLPi3D
{

#ifndef M_PI
#define M_PI           3.14159265358979323846
#endif
#define LOG_TAG "Main"

#define LOG_TAGQI    __FILE__ ":" STR(__LINE__)
#define STR(x) STRINGIFY(x)
#define STRINGIFY(x) # x

#define MAX_CHAR 64

//! The depth of the projection matrix stack.
#define MAX_PROJECTION_MATRIX	2

//! The depth of the model matrix stack.
#define MAX_MODEL_MATRIX	8

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
#define MAX_PROGRAM_SIZE 20
#define MAX_PATH_LEN 500

// Load shader from SDCARD using SHADER_FILE_SUPPORT 1
// Load shader from Source File in stringfy way using SHADER_FILE_SUPPORT 0
#define SHADER_FILE_SUPPORT 0

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
   TriangleType,
   RectangleType,
   QuadType,
   ObjFileType,
   FontEnglish,
   FontPunjabi,
   FontThai,
   FontArabic,
   FontTamil,
   FontType
};

enum CameraType{
	//! The Ortho Camera type identifier.
	ortho = 0,

	//! The Perespective Camera type identifier.
	perespective
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
};

#endif
