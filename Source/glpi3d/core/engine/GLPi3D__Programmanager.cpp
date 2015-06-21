#include "GLPi3D__Programmanager.h"
#include <string.h>
#include "GLPi3D__FileBuffer.h"
#include "GLPi3D__Glutils.h"

namespace GLPi3D
{

ProgramManager::ProgramManager(void) {
	for (int idx = 0; idx < MAX_PROGRAM_SIZE; idx++) {
		programList[idx] = NULL;
	}
	programCounter = 0;
}

ProgramManager::~ProgramManager(void) {
	for (int idx = 0; idx < MAX_PROGRAM_SIZE; idx++) {
		free (programList[idx]);
	}
	programCounter = 0;
}

/*!
 Initialize a new tstProgram structure.

 \param[in] name The internal name to use for the new tstProgram.

 \return Return a new tstProgram structure pointer.
 */
tstProgram* ProgramManager::ProgramInit(::int8 *name) {
	tstProgram *program = (tstProgram *) calloc(1, sizeof(tstProgram));

	strcpy((char*)program->name, (char*)name);
	program->ProgramID = 0;

	return program;
}

/*!
 Free a previously initialized tstProgram structure.

 \param[in,out] program A valid tstProgram structure pointer.

 \return Return a NULL tstProgram structure pointer.
 */
tstProgram* ProgramManager::ProgramFree(tstProgram *program) {
	if (program->UniformArray)
		free(program->UniformArray);

	if (program->VertexAttribArray)
		free(program->VertexAttribArray);

	if (program->ProgramID)
		ProgramDeleteId(program);

	free(program);
	return NULL;
}

/*!
 Helper function to fully create a tstProgram sturcture.

 \param[in] name The internal name to use for the tstProgram.
 \param[in] VertexShader_filename The vertex shader file to load.
 \param[in] fragment_shader_filename The fragment shader file to load.
 \param[in] relative_path Determine if the VertexShader_filename and the fragment_shader_filename are relative or absolute path.
 \param[in] debug_shader Enable (1) or disable (0) debugging functionalities while compiling the shaders file and linking the program.
 \param[in] ProgramBindAttribCallback The program bind attribute callback. (Called before the linking phase.)
 \param[in] ProgramDrawCallback	The program draw callback. (Called everytime the program is about to be use for drawing.)

 \return Return a new tstProgram structure pointer upon successfull loading, compiling and linking, else return NULL.
 */
tstProgram* ProgramManager::ProgramCreate(::int8 *name, ::int8 *VertexShader_filename, ::int8 *fragment_shader_filename, unsigned char relative_path,
		unsigned char debug_shader, PROGRAMBINDATTRIBCALLBACK *ProgramBindAttribCallback, PROGRAMDRAWCALLBACK *ProgramDrawCallback) {
	tstProgram *program = ProgramInit(name);

	tstGlFileCache *m = reserveCache( VertexShader_filename, relative_path);

	if (m) {
		program->VertexShader = ShaderManager::ShaderInit(VertexShader_filename,
				GL_VERTEX_SHADER);

		ShaderManager::ShaderCompile(program->VertexShader, (char *) m->buffer,
				debug_shader);

		freeCache(m);
	}

	m = reserveCache( fragment_shader_filename, relative_path);

	if (m) {
		program->FragmentShader = ShaderManager::ShaderInit(
				fragment_shader_filename, GL_FRAGMENT_SHADER);

		ShaderManager::ShaderCompile(program->FragmentShader,
				(char *) m->buffer, debug_shader);

		freeCache(m);
	}

	program->ProgramBindAttribCallback = ProgramBindAttribCallback;

	program->ProgramDrawCallback = ProgramDrawCallback;

	ProgramLink(program, debug_shader);

	return program;
}

/*!
 Internal function used to add a new uniform into a tstProgram uniform database.

 \param[in,out] program A valid tstProgram structure pointer.
 \param[in] name The name of the uniform.
 \param[in] type The variable type for this uniform.

 \return Return the newly created UNIFORM index inside the tstProgram uniform database.
 */
unsigned char ProgramManager::ProgramAddUniform(tstProgram *program, char *name,
		unsigned int type) {
	unsigned char uniform_index = program->UniformCount;

	++program->UniformCount;

	program->UniformArray = (tstUniform *) realloc(program->UniformArray,
			program->UniformCount * sizeof(tstUniform));

	memset(&program->UniformArray[uniform_index], 0, sizeof(tstUniform));

	strcpy(program->UniformArray[uniform_index].name, name);

	program->UniformArray[uniform_index].type = type;

	program->UniformArray[uniform_index].location = glGetUniformLocation(
			program->ProgramID, name);

	return uniform_index;
}

/*!
 Internal function used to add a new uniform into a tstProgram vertex attribute database.

 \param[in,out] program A valid tstProgram structure pointer.
 \param[in] name The name of the vertex attribute.
 \param[in] type The variable type for this attribute.

 \return Return the newly created tstVertexArrib index inside the tstProgram vertex attribute database.
 */
unsigned char ProgramManager::ProgramAddVertexAttrib(tstProgram *program,
		char *name, unsigned int type) {
	unsigned char tstVertexArrib_index = program->VertexAttribCount;

	++program->VertexAttribCount;

	program->VertexAttribArray = (tstVertexArrib *) realloc(
			program->VertexAttribArray,
			program->VertexAttribCount * sizeof(tstVertexArrib));

	memset(&program->VertexAttribArray[tstVertexArrib_index], 0,
			sizeof(tstVertexArrib));

	strcpy(program->VertexAttribArray[tstVertexArrib_index].name, name);

	program->VertexAttribArray[tstVertexArrib_index].type = type;

	program->VertexAttribArray[tstVertexArrib_index].location =
			glGetAttribLocation(program->ProgramID, name);

	return tstVertexArrib_index;
}

/*!
 Link the shader program.

 \param[in] program A valid tstProgram structure pointer with vertex and fragment shader code loaded.
 \param[in] debug Determine if you need debugging functionalities while processing the linking stage of the shader.

 \return Return 1 if the shader program have succesfully been linked, else return 0.
 */
unsigned char ProgramManager::ProgramLink(tstProgram *program, unsigned char debug) {
   unsigned int i = 0, type;

   char *log, name[MAX_CHAR];

   int status, len, total, size;

   if (program->ProgramID)
      return 1;

   program->ProgramID = glCreateProgram();

   glAttachShader(program->ProgramID, program->VertexShader->sid);

   glAttachShader(program->ProgramID, program->FragmentShader->sid);

   if (program->ProgramBindAttribCallback)
      program->ProgramBindAttribCallback(program);

   glLinkProgram(program->ProgramID);

   if (debug) {
      glGetProgramiv(program->ProgramID, GL_INFO_LOG_LENGTH, &len);

      if (len) {
         log = (char *) malloc(len);

         glGetProgramInfoLog(program->ProgramID, len, &len, log);

         printf("[ %s ]\n%s", program->name, log );

         free(log);
      }
   }

   glGetProgramiv(program->ProgramID, GL_LINK_STATUS, &status);

   if (!status) {
      ProgramDeleteId(program);
      return 0;
   }

   if (debug) {
      glValidateProgram(program->ProgramID);

      glGetProgramiv(program->ProgramID, GL_INFO_LOG_LENGTH, &len);

      if (len) {
         log = (char *) malloc(len);

         glGetProgramInfoLog(program->ProgramID, len, &len, log);

         printf("[ %s ]\n%s", program->name, log );
         free(log);
      }

      glGetProgramiv(program->ProgramID, GL_VALIDATE_STATUS, &status);

      if (!status){
         ProgramDeleteId(program);
         return 0;
      }
   }

   glGetProgramiv(program->ProgramID, GL_ACTIVE_ATTRIBUTES, &total);

   i = 0;
   while (i != total) {
      glGetActiveAttrib(program->ProgramID, i, MAX_CHAR, &len, &size, &type,
            name);

      ProgramAddVertexAttrib(program, name, type);

      ++i;
   }

   glGetProgramiv(program->ProgramID, GL_ACTIVE_UNIFORMS, &total);

   i = 0;
   while (i != total) {
      glGetActiveUniform(program->ProgramID, i, MAX_CHAR, &len, &size, &type,
            name);

      ProgramAddUniform(program, name, type);

      ++i;
   }

   return 1;
}

/*!
 Set the tstProgram draw callback.

 \param[in,out] program A valid tstProgram structure pointer.
 \param[in] ProgramDrawCallback A valid PROGRAMDRAWCALLBACK function pointer.

 */
void ProgramManager::ProgramSetDrawCallBack(tstProgram *program,
		PROGRAMDRAWCALLBACK *ProgramDrawCallback) {
	program->ProgramDrawCallback = ProgramDrawCallback;
}

/*!
 Set the tstProgram bind attribute location callback.

 \param[in,out] program A valid tstProgram structure pointer.
 \param[in] ProgramBindAttribCallback A valid PROGRAMBINDATTRIBCALLBACK function pointer.

 */
void ProgramManager::ProgramSetBindAttribLocationLocationCallback(
		tstProgram *program,
		PROGRAMBINDATTRIBCALLBACK *ProgramBindAttribCallback) {
	program->ProgramBindAttribCallback = ProgramBindAttribCallback;
}

/*!
 Retrieve a vertex attribute location.

 \param[in] program A valid tstProgram structure pointer.
 \param[in] name The name of the vertex attribute.

 \return Return the vertex attribute location.
 */
char ProgramManager::ProgramGetVertexAttribLocation(tstProgram *program,
		char *name) {
	unsigned char i = 0;

	while (i != program->VertexAttribCount) {
		if (!strcmp(program->VertexAttribArray[i].name, name)) {
			return program->VertexAttribArray[i].location;
		}

		++i;
	}

	return -1;
}

/*!
 Retrieve a uniform location.

 \param[in] program A valid tstProgram structure pointer.
 \param[in] name The name of the uniform.

 \return Return the uniform location.
 */
GLint ProgramManager::ProgramGetUniformLocation(tstProgram *program, char *name) {
	unsigned char i = 0;

	while (i != program->UniformCount) {
		if (!strcmp(program->UniformArray[i].name, name)) {
			return program->UniformArray[i].location;
		}

		++i;
	}

	return -1;
}

/*!
 Delete the GLSL program id attached to the tstProgram structure.

 \param[in,out] program A valid tstProgram structure pointer.
 */
void ProgramManager::ProgramDeleteId(tstProgram *program) {
	if (program->ProgramID) {
		glDeleteProgram(program->ProgramID);

		program->ProgramID = 0;
	}
}

/*!
 Set a tstProgram for drawing. If a ProgramDrawCallback is attached to the structure
 the execution pointer will be redirected to it so you can set/upadte the uniforms
 variable for the current tstProgram.

 \param[in,out] program A valid tstProgram structure pointer.
 */
void ProgramManager::ProgramDraw(tstProgram *program) {
	glUseProgram(program->ProgramID);

	if (program->ProgramDrawCallback)
		program->ProgramDrawCallback(program);
}

/*!
 Helper function to load and compile a  shader file.

 \param[in,out] program A valid tstProgram structure pointer.
 \param[in] ProgramBindAttribCallback A valid PROGRAMBINDATTRIBCALLBACK function pointer.
 \param[in] ProgramDrawCallback A valid PROGRAMDRAWCALLBACK function pointer.
 \param[in] filename The filename of the  file.
 \param[in] debug_shader Enable or disable debugging functionalities.
 \param[in] relative_path Determine of the filename is a relative (1) or absolute (0) path.

 \return Return 1 if the tstProgram have successfully loaded and compiled the  shader file.
 */

unsigned char ProgramManager::ProgramLoad(tstProgram *program,
		PROGRAMBINDATTRIBCALLBACK *ProgramBindAttribCallback,
      PROGRAMDRAWCALLBACK *ProgramDrawCallback, ::int8 *filename,
		unsigned char debug_shader, unsigned char relative_path) {
	tstGlFileCache *m = reserveCache( filename, relative_path);

	if (m) {
		char vertex_token[MAX_CHAR] = { "GL_VERTEX_SHADER" },
				fragment_token[MAX_CHAR] = { "GL_FRAGMENT_SHADER" },
				*VertexShader = strstr((char *) m->buffer, vertex_token),
				*FragmentShader = strstr((char *) m->buffer, fragment_token);

		GLUtils::extractFileName(filename, program->name);

		if ((VertexShader && FragmentShader)
				&& (FragmentShader > VertexShader)) {
			program->VertexShader = ShaderManager::ShaderInit(program->name,
					GL_VERTEX_SHADER);

			VertexShader += strlen(vertex_token);

			*FragmentShader = 0;

			ShaderManager::ShaderCompile(program->VertexShader, VertexShader,
					debug_shader);

			program->FragmentShader = ShaderManager::ShaderInit(program->name,
					GL_FRAGMENT_SHADER);

			FragmentShader += strlen(fragment_token);

			ShaderManager::ShaderCompile(program->FragmentShader,
					FragmentShader, debug_shader);

			program->ProgramBindAttribCallback = ProgramBindAttribCallback;

			program->ProgramDrawCallback = ProgramDrawCallback;

			ProgramLink(program, debug_shader);
		}

		freeCache(m);

		return 1;
	}

	return 0;
}

/*!
 \Return the program at specified index of list.

 \param[in,out] program A valid tstProgram structure pointer.

 \return Return 0 if value already present, 1 if added successfully, 2 not added due to overflow.
 */
tstProgram* ProgramManager::Program(::int8* programName) {
	for (int idx = 0; idx < MAX_PROGRAM_SIZE; idx++) {
		if (programList[idx] && !strcmp((char*)programList[idx]->name, (char*)programName)) {
			return programList[idx];
		}
	}
	return NULL;
}

/*!
 \Add the Program into buffer.

 \param[in,out] program A valid tstProgram structure pointer.

 \return Return 0 if value already present, 1 if added successfully, 2 not added due to overflow.
 */
unsigned char ProgramManager::AddProgram(tstProgram* program) {
	bool isPresent = false;
	for (int idx = 0; idx < MAX_PROGRAM_SIZE; idx++) {
		if (programList[idx]
				&& !strcmp((char*)programList[idx]->name, (char*)program->name)) {
			isPresent = true;
			//Already present in the list
			return 0;
		}
	}

	if (!isPresent) {
		for (int idx = 0; idx < MAX_PROGRAM_SIZE; idx++) {
			if (!programList[idx]) {
				programList[idx] = program;
				programCounter++;
				return 1; // List added sucessfully
			}
		}
	}
	return 2; // No Space in the list to add
}

/*!
 \Remove the program from buffer.

 \param[in,out] program A valid tstProgram structure pointer.

 \return Return 1 if the tstProgram have successfully removed else 0 for no program found in the list.
 */
unsigned char ProgramManager::RemoveProgram(tstProgram* program) {
	for (int idx = 0; idx < MAX_PROGRAM_SIZE; idx++) {
		if (programList[idx]
				&& !strcmp((char*)programList[idx]->name, (char*)program->name)) {
			free (programList[idx]);
			programCounter--;
			programList[idx] = NULL;
			return 1;
		}
	}
	return 0;
}
}

