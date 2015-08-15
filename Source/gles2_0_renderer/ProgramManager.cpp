#include "ProgramManager.h"
#include <string.h>
#include "Cache.h"
#include "glutils.h"

// Static member variables initialiazation; Temporary solution globals should be removed with elegant data structs.
ProgramManager* ProgramManager::singleTon = NULL;
PROGRAM* ProgramManager::programList[50]; 
unsigned char ProgramManager::programCounter;

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
 Initialize a new PROGRAM structure.

 \param[in] name The internal name to use for the new PROGRAM.

 \return Return a new PROGRAM structure pointer.
 */
PROGRAM* ProgramManager::ProgramInit(char *name) {
	PROGRAM *program = (PROGRAM *) calloc(1, sizeof(PROGRAM));

	strcpy(program->name, name);
	program->ProgramID = 0;

	return program;
}

/*!
 Free a previously initialized PROGRAM structure.

 \param[in,out] program A valid PROGRAM structure pointer.

 \return Return a NULL PROGRAM structure pointer.
 */
PROGRAM* ProgramManager::ProgramFree(PROGRAM *program) {
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
 Helper function to fully create a PROGRAM sturcture.

 \param[in] name The internal name to use for the PROGRAM.
 \param[in] VertexShader_filename The vertex shader file to load.
 \param[in] fragment_shader_filename The fragment shader file to load.
 \param[in] relative_path Determine if the VertexShader_filename and the fragment_shader_filename are relative or absolute path.
 \param[in] debug_shader Enable (1) or disable (0) debugging functionalities while compiling the shaders file and linking the program.
 \param[in] ProgramBindAttribCallback The program bind attribute callback. (Called before the linking phase.)
 \param[in] ProgramDrawCallback	The program draw callback. (Called everytime the program is about to be use for drawing.)

 \return Return a new PROGRAM structure pointer upon successfull loading, compiling and linking, else return NULL.
 */
PROGRAM* ProgramManager::ProgramCreate(char *name, char *VertexShader_filename,
		char *fragment_shader_filename, unsigned char relative_path,
		unsigned char debug_shader,
		PROGRAMBINDATTRIBCALLBACK *ProgramBindAttribCallback,
		PROGRAMDRAWCALLBACK *ProgramDrawCallback) {
	PROGRAM *program = ProgramInit(name);

	BUFFER *m = reserveCache( VertexShader_filename, relative_path);

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
 Internal function used to add a new uniform into a PROGRAM uniform database.

 \param[in,out] program A valid PROGRAM structure pointer.
 \param[in] name The name of the uniform.
 \param[in] type The variable type for this uniform.

 \return Return the newly created UNIFORM index inside the PROGRAM uniform database.
 */
unsigned char ProgramManager::ProgramAddUniform(PROGRAM *program, char *name,
		unsigned int type) {
	unsigned char uniform_index = program->UniformCount;

	++program->UniformCount;

	program->UniformArray = (UNIFORM *) realloc(program->UniformArray,
			program->UniformCount * sizeof(UNIFORM));

	memset(&program->UniformArray[uniform_index], 0, sizeof(UNIFORM));

	strcpy(program->UniformArray[uniform_index].name, name);

	program->UniformArray[uniform_index].type = type;

	program->UniformArray[uniform_index].location = glGetUniformLocation(
			program->ProgramID, name);

	return uniform_index;
}

/*!
 Internal function used to add a new uniform into a PROGRAM vertex attribute database.

 \param[in,out] program A valid PROGRAM structure pointer.
 \param[in] name The name of the vertex attribute.
 \param[in] type The variable type for this attribute.

 \return Return the newly created VERTEX_ATTRIB index inside the PROGRAM vertex attribute database.
 */
unsigned char ProgramManager::ProgramAddVertexAttrib(PROGRAM *program,
		char *name, unsigned int type) {
	unsigned char vertex_attrib_index = program->VertexAttribCount;

	++program->VertexAttribCount;

	program->VertexAttribArray = (VERTEX_ATTRIB *) realloc(
			program->VertexAttribArray,
			program->VertexAttribCount * sizeof(VERTEX_ATTRIB));

	memset(&program->VertexAttribArray[vertex_attrib_index], 0,
			sizeof(VERTEX_ATTRIB));

	strcpy(program->VertexAttribArray[vertex_attrib_index].name, name);

	program->VertexAttribArray[vertex_attrib_index].type = type;

	program->VertexAttribArray[vertex_attrib_index].location =
			glGetAttribLocation(program->ProgramID, name);

	return vertex_attrib_index;
}

/*!
 Link the shader program.

 \param[in] program A valid PROGRAM structure pointer with vertex and fragment shader code loaded.
 \param[in] debug Determine if you need debugging functionalities while processing the linking stage of the shader.

 \return Return 1 if the shader program have succesfully been linked, else return 0.
 */
unsigned char ProgramManager::ProgramLink(PROGRAM *program,
		unsigned char debug) {
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

#ifdef __IPHONE_4_0

			printf("[ %s ]\n%s", program->name, log );
#else
#ifdef _WIN32
			printf("[ %s ]\n%s", program->name, log );
#else
			__android_log_print(ANDROID_LOG_ERROR, "", "[ %s ]\n%s",
					program->name, log);
#endif
#endif

			free(log);
		}
	}

	glGetProgramiv(program->ProgramID, GL_LINK_STATUS, &status);

	if (!status) {
		goto delete_program;
	}

	if (debug) {
		glValidateProgram(program->ProgramID);

		glGetProgramiv(program->ProgramID, GL_INFO_LOG_LENGTH, &len);

		if (len) {
			log = (char *) malloc(len);

			glGetProgramInfoLog(program->ProgramID, len, &len, log);

			//Parminder: Need a preprocessor flag for Android case at present #else is being used for Android
#ifdef __IPHONE_4_0
			printf("[ %s ]\n%s", program->name, log );
#else
#ifdef _WIN32
			printf("[ %s ]\n%s", program->name, log );
#else
			__android_log_print(ANDROID_LOG_ERROR, "", "[ %s ]\n%s",
					program->name, log);
#endif
#endif

			free(log);
		}

		glGetProgramiv(program->ProgramID, GL_VALIDATE_STATUS, &status);

		if (!status)
			goto delete_program;
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

	delete_program:

	ProgramDeleteId(program);

	return 0;
}

/*!
 Set the PROGRAM draw callback.

 \param[in,out] program A valid PROGRAM structure pointer.
 \param[in] ProgramDrawCallback A valid PROGRAMDRAWCALLBACK function pointer.

 */
void ProgramManager::ProgramSetDrawCallBack(PROGRAM *program,
		PROGRAMDRAWCALLBACK *ProgramDrawCallback) {
	program->ProgramDrawCallback = ProgramDrawCallback;
}

/*!
 Set the PROGRAM bind attribute location callback.

 \param[in,out] program A valid PROGRAM structure pointer.
 \param[in] ProgramBindAttribCallback A valid PROGRAMBINDATTRIBCALLBACK function pointer.

 */
void ProgramManager::ProgramSetBindAttribLocationLocationCallback(
		PROGRAM *program,
		PROGRAMBINDATTRIBCALLBACK *ProgramBindAttribCallback) {
	program->ProgramBindAttribCallback = ProgramBindAttribCallback;
}

/*!
 Retrieve a vertex attribute location.

 \param[in] program A valid PROGRAM structure pointer.
 \param[in] name The name of the vertex attribute.

 \return Return the vertex attribute location.
 */
char ProgramManager::ProgramGetVertexAttribLocation(PROGRAM *program,
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

char ProgramManager::ProgramGetVertexAttribLocation(unsigned int ID, char *name) {
	return ProgramGetVertexAttribLocation(Program(ID), name);
}


/*!
 Retrieve a uniform location.

 \param[in] program A valid PROGRAM structure pointer.
 \param[in] name The name of the uniform.

 \return Return the uniform location.
 */
char ProgramManager::ProgramGetUniformLocation(PROGRAM *program, char *name) {
	unsigned char i = 0;

	while (i != program->UniformCount) {
		if (!strcmp(program->UniformArray[i].name, name)) {
			return program->UniformArray[i].location;
		}

		++i;
	}

	return -1;
}

char ProgramManager::ProgramGetUniformLocation(unsigned int ID, char *name) {
	return ProgramGetUniformLocation(Program(ID), name);
}


/*!
 Delete the GLSL program id attached to the PROGRAM structure.

 \param[in,out] program A valid PROGRAM structure pointer.
 */
void ProgramManager::ProgramDeleteId(PROGRAM *program) {
	if (program->ProgramID) {
		glDeleteProgram(program->ProgramID);

		program->ProgramID = 0;
	}
}

/*!
 Set a PROGRAM for drawing. If a ProgramDrawCallback is attached to the structure
 the execution pointer will be redirected to it so you can set/upadte the uniforms
 variable for the current PROGRAM.

 \param[in,out] program A valid PROGRAM structure pointer.
 */
void ProgramManager::ProgramDraw(PROGRAM *program) {
	glUseProgram(program->ProgramID);

	if (program->ProgramDrawCallback)
		program->ProgramDrawCallback(program);
}

/*!
 Helper function to load and compile a  shader file.

 \param[in,out] program A valid PROGRAM structure pointer.
 \param[in] name name of the shader
 \param[in] vertexShaderPath A valid vertex shader path.
 \param[in] fragmentShaderPath A valid fragment shader path.
 \param[in] debug_shader Enable or disable debugging functionalities.
 \param[in] relative_path Determine of the filename is a relative (1) or absolute (0) path.

 \return Return 1 if the PROGRAM have successfully loaded and compiled the  shader file.
 */

PROGRAM* ProgramManager::LoadShader(char* name, char* vertexShaderPath, char* fragmentShaderPath )
{
   PROGRAM* program;
   if (! ( program = Program( name ) ) )
   {
      program = ProgramInit( name );
      AddProgram( program );
      program->VertexShader	= ShaderManager::ShaderInit( vertexShaderPath,	GL_VERTEX_SHADER	);
      program->FragmentShader	= ShaderManager::ShaderInit( fragmentShaderPath, GL_FRAGMENT_SHADER	);

        /////////// Vertex shader //////////////////////////
        BUFFER *m = reserveCache( vertexShaderPath, true );

        if( m ) {
            if( !ShaderManager::ShaderCompile( program->VertexShader, ( char * )m->buffer, 1 ) ) exit( 1 );
            m = freeCache( m );
        }

        /////////// Fragment shader //////////////////////////
        m = reserveCache( fragmentShaderPath, true );
        if( m ) {
            if( !ShaderManager::ShaderCompile( program->FragmentShader, ( char * )m->buffer, 1 ) ) exit( 2 );
            m = freeCache( m );
        }

        if( !ProgramLink( program, 1 ) ) exit( 3 );
   }

   return program;
}



/*!
 \Return the program at specified index of list.

 \param[in,out] program A valid PROGRAM structure pointer.

 \return Return 0 if value already present, 1 if added successfully, 2 not added due to overflow.
 */
PROGRAM* ProgramManager::Program(char* programName) {
	for (int idx = 0; idx < MAX_PROGRAM_SIZE; idx++) {
		if (programList[idx] && !strcmp(programList[idx]->name, programName)) {
			return programList[idx];
		}
	}
	return NULL;
}

/*!
 \Return the program at specified index of list.

 \param[in,out] program A valid tstProgram structure pointer.

 \return Return 0 if value already present, 1 if added successfully, 2 not added due to overflow.
 */
PROGRAM* ProgramManager::Program(unsigned int ID) {
	for (int idx = 0; idx < MAX_PROGRAM_SIZE; idx++) {
		if (programList[idx] && (programList[idx]->ProgramID == ID)) {
			return programList[idx];
		}
	}
	return NULL;
}
/*!
 \Add the Program into buffer.

 \param[in,out] program A valid PROGRAM structure pointer.

 \return Return 0 if value already present, 1 if added successfully, 2 not added due to overflow.
 */
unsigned char ProgramManager::AddProgram(PROGRAM* program) {
	bool isPresent = false;
	for (int idx = 0; idx < MAX_PROGRAM_SIZE; idx++) {
		if (programList[idx]
				&& !strcmp(programList[idx]->name, program->name)) {
			isPresent = true;
			//Already present in the list
			return 0;
		}
	}

	if (!isPresent) {
		bool added = false;
		for (int idx = 0; idx < MAX_PROGRAM_SIZE; idx++) {
			if (!programList[idx]) {
				programList[idx] = program;
				programCounter++;
				return 1; // List added sucessfully
			}
		}
		return 2; // No Space in the list to add
	}
}

/*!
 \Remove the program from buffer.

 \param[in,out] program A valid PROGRAM structure pointer.

 \return Return 1 if the PROGRAM have successfully removed else 0 for no program found in the list.
 */
unsigned char ProgramManager::RemoveProgram(PROGRAM* program) {
	for (int idx = 0; idx < MAX_PROGRAM_SIZE; idx++) {
		if (programList[idx]
				&& !strcmp(programList[idx]->name, program->name)) {
			free (programList[idx]);
			programCounter--;
			programList[idx] = NULL;
			return 1;
		}
	}
	return 0;
}

 ProgramManager* ProgramManager::GetInstance(){ 
   if(!singleTon){ 
      singleTon = new ProgramManager();
   }
   return singleTon; 
}

 void ProgramManager::DeleteInstance(){ 

   for (int idx = 0; idx < MAX_PROGRAM_SIZE; idx++) {
      if (programList[idx]) {
         free (programList[idx]);
         programCounter--;
         programList[idx] = NULL;
      }
   }

   delete singleTon;
   singleTon = NULL;
}
