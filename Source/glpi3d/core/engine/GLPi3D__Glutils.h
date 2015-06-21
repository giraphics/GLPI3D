#ifndef GLUTILS_H
#define GLUTILS_H
#include "GLPi3D__Constant.h"

namespace GLPi3D
{

class GLUtils
{
public:
    GLUtils();

    static int checkForOpenGLError(const char *, int);
    static void printGLInfo(bool printExtensions = false);
	
	//!	Replace the \ for / in a file path.
	static void modifyPath( ::int8 *path );

	//!
   static void extractPath( ::int8 *path, ::int8 *modifiedPath );


	//! Extract the file name from given file path.
    static void extractFileName( ::int8 *filepath, ::int8 *filename );

    // Get next next immediate power of 2. 
    static int nextPowerOf2 (int a);
};
}

#endif // GLUTILS_H
