#include "GLPi3D__Glutils.h"

#ifdef _WIN32
//#include <GL/glew.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#endif

namespace GLPi3D
{

GLUtils::GLUtils() {}

int GLUtils::checkForOpenGLError(const char * file, int line) {
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    int    retCode = 0;
    GLenum glErr;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
      printf("glError in file %s @ line %d: %s\n - errorcode = 0x%x\n", file, line, glErr);
      retCode = 1;
      glErr = glGetError();
    }
    return retCode;

}

void GLUtils::printGLInfo(bool printExtensions) {
	//printf("\nGL_VENDOR:      %s\n",                   ( char * )glGetString( GL_VENDOR     ) );
	//printf("\nGL_RENDERER:    %s\n"  ,                 ( char * )glGetString( GL_RENDERER   ) );
	//printf("\nGL_VERSION:     %s\n"  ,                 ( char * )glGetString( GL_VERSION    ) );
	//printf("\nGL_SHADING_LANGUAGE_VERSION:  %s\n"  ,   ( char * )glGetString( GL_SHADING_LANGUAGE_VERSION ) );
	//if (printExtensions){
	//	printf("\nGL_EXTENSIONS:  %s\n"  , ( char * )glGetString( GL_EXTENSIONS ) );
	//}
}

void GLUtils::modifyPath( ::int8 *path )
{
    unsigned int i = 0;
    unsigned int l = strlen( (const char*)path );
    
    while( i != l )
    {
        if( path[ i ] == '\\' ) {
            path[ i ] = '/';
        }
        ++i;
    }
}


/*!
 Extract path from a file path.
 
 \param[in] filepath The file path to use to extract the path.
 \param[in,out] path The return path.
 */
void GLUtils::extractPath( ::int8 *path, ::int8 *modifiedPath )
{
    ::int8 *f			= NULL;
    unsigned int p	= 0;
    
    modifyPath( path );
    
    f = (::int8*)strrchr( (char*)path, '/' );
    
    if( f )
    {
        p = (f - path) + 1;
        strncpy( (char*)modifiedPath, (char*)path, p );
        path[ p ] = 0;
    }
}


/*!
 Extract the file name from given file path.
 
 \param[in] path complete path contain file name.
 \param[in,out] filename extracted.
 
 */
void GLUtils::extractFileName( ::int8 *filepath, ::int8 *filename )
{
    char *t = NULL;
    
    modifyPath( filepath );
    
    t = strrchr( (char*)filepath, '/' );
    
    if( t ) strcpy( (char*)filename, t + 1 );
    else strcpy( (char*)filename, (char*)filepath );
}
/*!
 Get next best fit from the specified current 'number'.
 
 \param[in] number, input number.
 \param[in,out] next immediate power of two.
 
 */
int GLUtils::nextPowerOf2 (int number)
{
    int rval = 1;
    while (rval<number) rval*=2;
    return rval;
}

}

