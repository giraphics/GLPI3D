/***************************************************************************
*================== Copyright by Continental Automotive GmbH ===============
****************************************************************************
* Titel        : GLPi3D1ci.h
*
* Description  : internal configuration of GLPi OpenGL ES Framework
*
* Environment  : HW and OS independent
*
* Responsible  : Parminder Singh
*
* Guidelines   : SMK V4.9
*
* Template name: OOLite Class, Revision 1.0
*
* Revision list:
* Archive $Log: GLPi1ci.h  $
* 
****************************************************************************/
#ifndef GLPI3DCI_H
#define GLPI3DCI_H

extern "C"
{
    #include "exea_c1.h"
	#include "cdef.h"
}

#include "GRASG_ce.h"

#if   ( defined _MSC_VER && !defined TOGETHER )

#	define GLPi3D_DEBUG (On)

#	define GLPi3D_vThrowException   __debugbreak( ) 

#	if GLPi3D_DEBUG
#		define GLPi3D_ERROR( a ) printf( a )
#		define GLPi3D_ASSERT( a ) if (!(a)) { GLPi3D_vThrowException; }
#	else
#		define GLPi3D_ERROR( a ) 
#		define GLPi3D_ASSERT( a )  
#	endif

#elif   ( defined __ghs__ && !defined QAC_Analysis && !defined QACPP_ANALYSIS_ONLY )  

#	define GLPi3D_DEBUG (On)

#	define GLPi3D_vThrowException  // EXEA_vHandleException((EXEA_tenModuleID)GLPi3D_ID, (EXEA_tenErrorID)(192), __LINE__, 0)

#	if GLPi3D_DEBUG
#		define GLPi3D_ERROR( a ) printf( a )
#		define GLPi3D_ASSERT( a ) if (!(a)) { GLPi3D_vThrowException; }
#	else
#		define GLPi3D_ERROR( a ) 
#		define GLPi3D_ASSERT( a )  
#	endif

#else

#	define GLPi3D_vThrowException   
#	define GLPi3D_ERROR( a ) 
#	define GLPi3D_ASSERT( a )  

#endif

#endif // GLPI3D
