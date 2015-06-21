#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#pragma once

//! Header File inclusion
#include "GLPi3D__Constant.h"

namespace GLPi3D
{

//! Structure to handle shading language shader object.
typedef struct
{
	//! Name assigned to shader.
   ::int8			name[ MAX_CHAR ];

	//! Shader type. [GL_VERTEX_SHADER or GL_FRAGMENT_SHADER]
	unsigned int	type;

	//! The Shader's unique ID.
	unsigned int	sid;

} tenShader;


class ShaderManager
{

public:
    //! Constructor Shader class.
	ShaderManager(void);

    //! Destructor Shader class.
	~ShaderManager(void);

    //! Create a new shader of type mentioned.
   //static tenShader*  ShaderInit( ::int8 *name, unsigned int type );
   static tenShader*  ShaderInit( ::int8 *name, unsigned int type);

    //! Compilation process for Shader.
	static unsigned char	ShaderCompile( tenShader *shader, const char *code, unsigned char debug );

    //! Free the Shader.
	static void			    ShaderDelete( tenShader *shader );
};
}


#endif
