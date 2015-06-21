#ifdef _WIN32
#endif

#include "GLPi3D__FileBuffer.h"

#include <string.h>
#include <fstream>
using std::ifstream;
#include <sstream>
using std::ostringstream;
#include "GLPi3D__Glutils.h"

namespace GLPi3D
{

/*!
	Open/Extract a file from disk and load it in cachePtr.

	\param[in] filename The file to load in cachePtr.
	\param[in] relative_path Determine if the filename is an absolute or relative path.

	\return Return a tstGlFileCache structure pointer if the file is found and loaded, instead will return
	NULL.
*/
tstGlFileCache *reserveCache( ::int8 *filename, ::uint8 relative_path )
{
   // Load contents of file into shaderCode here…
   ifstream inFile( (char*)filename, ifstream::in );
   if( !inFile ) {
   fprintf(stderr, "Error opening file: %s\n", filename );
   exit(1);
   }
   tstGlFileCache *cachePtr = ( tstGlFileCache * ) calloc( 1, sizeof( tstGlFileCache ) );
   cachePtr->buffer = (unsigned char *)malloc(10000);
   memset(cachePtr->buffer, 0, 10000);
   int i = 0;
   
   while( inFile.good() ) {
   int c = inFile.get();
      cachePtr->buffer[i++] = c;
   }
   inFile.close();

   cachePtr->buffer[--i] = '\0';
   strcpy( cachePtr->filename, (char*)filename );
   cachePtr->position = 0;
   cachePtr->size	 = 10000;
   cachePtr->buffer[ 10000-1 ] = 0;
   return cachePtr;
}

/*!
	Delete initialized tstGlFileCache.

	\param[in,out] cachePtr A valid tstGlFileCache structure object.

	\return Return a NULL tstGlFileCache pointer.
*/
tstGlFileCache *freeCache( tstGlFileCache *cachePtr )
{
	if( cachePtr->buffer ) free( cachePtr->buffer );

	free( cachePtr );
	return NULL;
}
}

