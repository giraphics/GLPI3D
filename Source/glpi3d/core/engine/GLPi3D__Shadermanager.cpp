#include "GLPi3D__Shadermanager.h"
#include <stdio.h>
#include "GLPi3D__Glutils.h"
#include "GLPi3D__FileBuffer.h"

#include <stdlib.h>
#include <string.h>
namespace GLPi3D
{

/*!
	Default Constructor ShaderManager
	
	\param[in] NONE.
	
	\return NONE.
*/
ShaderManager::ShaderManager(void)
{
}

/*!
	Default Destructor ShaderManager
	
	\param[in] NONE.
	
	\return NONE.
*/
ShaderManager::~ShaderManager(void)
{
}

/*!
	Create a new tenShader structure.
	
	\param[in] name The internal name for the new SHADER structure pointer.
	\param[in] type The type of shader the structure will deal with. (Either GL_VERTEX_SHADER or
	GL_FRAGMENT_SHADER)
	
	\return Return a new SHADER structure pointer.
*/
//tenShader *ShaderManager::ShaderInit( ::int8 *name, unsigned int type )
//{
//	tenShader *shader = ( tenShader * ) calloc( 1, sizeof( tenShader ) );
//
//	strcpy( (char*)shader->name, (char*)name );
//
//	shader->type = type;
//	
//   tstGlFileCache *m = reserveCache( name, true );
//
//	if( m ) {
//		if( !ShaderManager::ShaderCompile( shader, ( char * )m->buffer, 1 ) ) exit( 1 );
//	   freeCache( m );
//   }
//
//	return shader;
//}

tenShader *ShaderManager::ShaderInit( ::int8 *src, unsigned int type)
{
   tenShader *shader = ( tenShader * ) calloc( 1, sizeof( tenShader ) );
   strcpy( (char*)shader->name, (char*)"" ); //Currently unused
   shader->type = type;

   #if SHADER_FILE_SUPPORT == 1
      tstGlFileCache *m = reserveCache( src, true );
      if( m ) {
         if( !ShaderManager::ShaderCompile( shader, ( char * )m->buffer, 1 ) ) exit( 1 );
         freeCache( m );
      }
   #else
      if( src ) {
         if( !ShaderManager::ShaderCompile( shader, ( char * )src, 1 ) ) exit( 1 );
      }
   #endif
   return shader;

   //if( shaderFileSupport ){
   //   tstGlFileCache *m = reserveCache( src, true );
   //   if( m ) {
   //      if( !ShaderManager::ShaderCompile( shader, ( char * )m->buffer, 1 ) ) exit( 1 );
   //      freeCache( m );
   //   }
   //}
   //else{
   //   if( src ) {
   //      if( !ShaderManager::ShaderCompile( shader, ( char * )src, 1 ) ) exit( 1 );
   //   }
   //}
}

/*!
	Compile a VERTEX or FRAGMENT shader code.
	
	\param[in,out] shader A valid SHADER structure pointer.
	\param[in] code The code to compile for the current SHADER.
	\param[in] debug Enable (1) or disable (0) debugging functionalities while compiling the shader.
	
	\return Return 1 if the shader code compile successfully, else return 0.
*/
unsigned char ShaderManager::ShaderCompile( tenShader *shader, const char *code, unsigned char debug )
{
   char type[ MAX_CHAR ] = {""};

   int loglen,
      status;

   if( shader->sid ) return 0;

   shader->sid = glCreateShader( shader->type );

    glShaderSource( shader->sid, 1, &code, NULL );

    glCompileShader( shader->sid );

   if( debug )
   {
      if( shader->type == GL_VERTEX_SHADER ) strcpy( type, "GL_VERTEX_SHADER" );
      else strcpy( type, "GL_FRAGMENT_SHADER" );
   	
      glGetShaderiv( shader->sid, GL_INFO_LOG_LENGTH, &loglen );
   	
      if( loglen )
      {
         char *log = ( char * ) malloc( loglen );

         glGetShaderInfoLog( shader->sid, loglen, &loglen, log );
         printf("[ %s:%s ]\n%s", shader->name, type, log );
         free( log );
      }
   }
    
    glGetShaderiv( shader->sid, GL_COMPILE_STATUS, &status );

	if( !status )
	{
		ShaderDelete( shader );
		return 0;
	}

	return 1;
}

/*!
	Delete the shader internal id maintained by GLES.
	
	\param[in,out] shader A valid tenShader structure pointer.
*/ 
void ShaderManager::ShaderDelete( tenShader *shader )
{
	if( shader->sid )
	{
		glDeleteShader( shader->sid );
		shader->sid = 0;
	}
}
}

