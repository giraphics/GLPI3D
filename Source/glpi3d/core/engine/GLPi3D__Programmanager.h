#ifndef PROGRAM_MANA_H
#define PROGRAM_MANA_H

#pragma once
#include "GLPi3D__Shadermanager.h"

namespace GLPi3D
{

//! Structure to deal with GLSL uniform variables.
typedef struct
{
	//! The name of the uniform.
	char			name[ MAX_CHAR ];

	//! The variable type for this uniform.
	unsigned int	type;

	//! The location id maintained by GLSL for this uniform.
	int				location;

	//! Determine if the uniform is constant or shoud be updated every frame.
	unsigned char	constant;

} tstUniform;


//! Structure to deal with vertex attribute variables.
typedef struct
{
	//! The name of the vertex attribute.
	char			name[ MAX_CHAR ];

	//! The variable type for this vertex attribute.
	unsigned int	type;

	//! The location of the id maintained GLSL for this vertex attribute.
	int				location;

} tstVertexArrib;


//! Draw callback function prototype.
typedef void( PROGRAMDRAWCALLBACK( void * ) );

//! Callback function prototypes for binding vertex attribute before compilation.
typedef void( PROGRAMBINDATTRIBCALLBACK( void * ) );


//! Structure to easily handle GLSL programs.
typedef struct
{
	//! The internal name to use for the shader program.
   ::int8						 name[ MAX_CHAR ];

	//! The vertex shader.
	tenShader             *VertexShader;

	//! The fragment shader.
	tenShader             *FragmentShader;

	//! The internal GLES program id for the shader program.
	unsigned int				 ProgramID;

	//! The number of uniforms.
	unsigned char				 UniformCount;

	//! Array of UNIFORM variables.
	tstUniform						 *UniformArray;

	//! The number of vertex attributes.
	unsigned char				 VertexAttribCount;

	//! Array of vertex attributes.
	tstVertexArrib				 *VertexAttribArray;

	//! The program draw callback.
	PROGRAMDRAWCALLBACK			 *ProgramDrawCallback;

	//! The bind attribute callback.
	PROGRAMBINDATTRIBCALLBACK	 *ProgramBindAttribCallback;

} tstProgram;


class ProgramManager
{
public:
	ProgramManager(void);
	~ProgramManager(void);

public:
   tstProgram *ProgramInit( ::int8 *name );

	tstProgram *ProgramFree( tstProgram *program );

   tstProgram *ProgramCreate( ::int8 *name, ::int8 *VertexShader_filename, ::int8 *fragment_shader_filename, unsigned char relative_path, unsigned char debug_shader, PROGRAMBINDATTRIBCALLBACK *ProgramBindAttribCallback, PROGRAMDRAWCALLBACK *ProgramDrawCallback );

	unsigned char ProgramLink( tstProgram *program, unsigned char debug );

	void ProgramSetDrawCallBack( tstProgram *program, PROGRAMDRAWCALLBACK *ProgramDrawCallback );

	void ProgramSetBindAttribLocationLocationCallback( tstProgram *program, PROGRAMBINDATTRIBCALLBACK *ProgramBindAttribCallback );

	char ProgramGetVertexAttribLocation( tstProgram *program, char *name );

	GLint ProgramGetUniformLocation( tstProgram *program, char *name );

	void ProgramDeleteId( tstProgram *program );

	void ProgramDraw( tstProgram *program );

	void ProgramReset( tstProgram *program );

   unsigned char ProgramLoad( tstProgram *program, PROGRAMBINDATTRIBCALLBACK	*ProgramBindAttribCallback, PROGRAMDRAWCALLBACK	*ProgramDrawCallback, ::int8 *filename, unsigned char	debug_shader, ::uint8 relative_path );

	unsigned char ProgramAddUniform( tstProgram *program, char *name, unsigned int type );

	unsigned char ProgramAddVertexAttrib( tstProgram *program, char *name, unsigned int type );

	tstProgram* Program( ::int8* program );

	unsigned char AddProgram( tstProgram* program );

	unsigned char RemoveProgram( tstProgram* program );

	inline unsigned char ProgramCount() { return programCounter; }

private:
	//! We need some mechanism to recognize the program for each model.
	//! Parminder temporarily using a array, must use a list instead.
	tstProgram* programList[20]; 

	//! Keeps track of the number of program in the renderer.
	unsigned char programCounter;
};
}

#endif
