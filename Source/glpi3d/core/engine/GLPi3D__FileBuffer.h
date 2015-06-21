#ifndef GRLC_CACHE_H
#define GRLC_CACHE_H
#ifdef _WIN32
//#include <GL/glew.h>
#else
#endif
#include "GLPi3D__Constant.h"
namespace GLPi3D
{

#define MAX_PATH	256

/*!
	\file Cache.h

	\brief Function prototypes and definitions to use with the tstGlFileCache structure.
*/


//! Structure for store Cached information.
typedef struct
{
	char			filename[ MAX_PATH ];

	unsigned int	size;

	unsigned int	position;

	unsigned char	*buffer;
} tstGlFileCache;


tstGlFileCache *reserveCache( ::int8 *filename, ::uint8 relative_path );

tstGlFileCache *freeCache( tstGlFileCache *cachePtr );
}

#endif
