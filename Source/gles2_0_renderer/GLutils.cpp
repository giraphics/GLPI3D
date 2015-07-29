#include "glutils.h"

GLUtilsGLES2::GLUtilsGLES2() {}

int GLUtilsGLES2::checkForOpenGLError(const char * file, int line) {
    //
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    //
    int    retCode = 0;
	#ifdef _WIN32
    GLenum glErr;

    glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n - errorcode = 0x%x\n", file, line, glErr);
        retCode = 1;
        glErr = glGetError();
    }
	#endif
    return retCode;

}

void GLUtilsGLES2::printGLInfo(bool printExtensions) {
	LOGI("\nGL_VENDOR:      %s\n", ( char * )glGetString( GL_VENDOR     ) );
	LOGI("\nGL_RENDERER:    %s\n"  , ( char * )glGetString( GL_RENDERER   ) );
	LOGI("\nGL_VERSION:     %s\n"  , ( char * )glGetString( GL_VERSION    ) );
	LOGI("\nGL_SHADING_LANGUAGE_VERSION:  %s\n"  , ( char * )glGetString( GL_SHADING_LANGUAGE_VERSION ) );
	if (printExtensions){
		LOGI("\nGL_EXTENSIONS:  %s\n"  , ( char * )glGetString( GL_EXTENSIONS ) );
	}
}

void GLUtilsGLES2::modifyPath( char *path )
{
    unsigned int i = 0;
    unsigned int l = (unsigned int)strlen( path );
    
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
void GLUtilsGLES2::extractPath( char *path, char *modifiedPath )
{
    char *f			= NULL;
    unsigned int p	= 0;
    
    modifyPath( path );
    
    f = strrchr( path, '/' );
    
    if( f )
    {
        p = (unsigned int)(f - path) + 1;
        strncpy( modifiedPath, path, p );
        path[ p ] = 0;
    }
}


/*!
 Extract the file name from given file path.
 
 \param[in] path complete path contain file name.
 \param[in,out] filename extracted.
 
 */
void GLUtilsGLES2::extractFileName( char *filepath, char *filename )
{
    char *t = NULL;
    
    modifyPath( filepath );
    
    t = strrchr( filepath, '/' );
    
    if( t ) strcpy( filename, t + 1 );
    else strcpy( filename, filepath );
}

/*!
 Get next best fit from the specified current 'number'.
 
 \param[in] number, input number.
 \param[in,out] next immediate power of two.
 
 */
int GLUtilsGLES2::nextPowerOf2 (int number)
{
    int rval = 1;
    while (rval<number) rval*=2;
    return rval;
}
