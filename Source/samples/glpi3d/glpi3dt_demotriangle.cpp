#include "glpi3dt_demotriangle.h"
#include "GLPi3D__Constant.h"

#if SHADER_FILE_SUPPORT == 1
#define VERTEX_SHADER_PRG        (int8 * )"pkg\\graphic\\glpi3d\\core\\samples\\shader\\TriangleVertex.glsl"
#define FRAGMENT_SHADER_PRG      (int8 * )"pkg\\graphic\\glpi3d\\core\\samples\\shader\\TriangleFragment.glsl"
#else
#define VERTEX_SHADER_PRG        (int8*)triangleVertex
#define FRAGMENT_SHADER_PRG      (int8*)triangleFragment
#endif

// Global variable for vertex position and color
static GLfloat gTriangleVertices[]  = { 0.0f, 0.1f, -0.1f, -0.1f, 0.1f, -0.1f };
static GLfloat gTriangleColors[]    = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };

// Global Object Declaration
const GLchar triangleVertex[] =
"                                                           \
attribute vec4 VertexPosition;                              \
attribute  vec4 VertexColor;                                \
uniform float  RadianAngle;                                 \
varying vec4       TriangleColor;                           \
mat2 r = mat2(cos(RadianAngle),sin(RadianAngle),     \
                     -sin(RadianAngle),cos(RadianAngle));   \
void main() {                                               \
    gl_Position   = mat4(r)*VertexPosition;                 \
    TriangleColor = VertexColor;                            \
}                                                           \
\0";

const GLchar triangleFragment[] =
"                                                           \
precision mediump float;                                    \
varying vec4       TriangleColor;                           \
void main() {                                               \
  gl_FragColor = TriangleColor;                             \
}                                                           \
\0";

/*!
   Simple Default Constructor

   \param[in] None.
   \return None

*/
DemoTriangle::DemoTriangle( ModelType typeOfModel, Model* parent ):Model(typeOfModel, parent)
{
   degree = 0;
}


/*!
	Simple Destructor

	\param[in] None.
	\return None

*/
DemoTriangle::~DemoTriangle()
{
   if ( program = ProgramManagerObj->Program( (::int8* )"Triangle" ) )
   {
      ProgramManagerObj->RemoveProgram(program);
   }
}

/*!
   Initialize the scene, reserve shaders, compile and chache program

   \param[in] None.
   \return None

*/
void DemoTriangle::InitModel()
{
   if (!(program = ProgramManagerObj->Program( (::int8*) "Triangle"))){
      program = ProgramManagerObj->ProgramInit( (::int8*) "Triangle" );
      ProgramManagerObj->AddProgram( program );
   }

   program->VertexShader   = ShaderManager::ShaderInit( VERTEX_SHADER_PRG, GL_VERTEX_SHADER );
   program->FragmentShader = ShaderManager::ShaderInit( FRAGMENT_SHADER_PRG, GL_FRAGMENT_SHADER);

   if( !ProgramManagerObj->ProgramLink( program, 1 ) ) exit( 3 );

   glUseProgram( program->ProgramID );
   positionAttribHandle = ProgramManagerObj->ProgramGetVertexAttribLocation(program,(char*)"VertexPosition");
   colorAttribHandle    = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"VertexColor");

   Model::InitModel();
   return;
}

/*!
   Initialize the scene, reserve shaders, compile and chache program

   \param[in] None.
   \return None

*/
void DemoTriangle::Render()
{
   //__START_PROFILING__
   glUseProgram( program->ProgramID );

   radian = degree += 0.2f/57.2957795f;
   // Query and send the uniform variable.
   radianAngle = glGetUniformLocation(program->ProgramID, "RadianAngle");
   glUniform1f(radianAngle, radian);

   glVertexAttribPointer(positionAttribHandle, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
   glVertexAttribPointer(colorAttribHandle, 3, GL_FLOAT, GL_FALSE, 0, gTriangleColors);
   glEnableVertexAttribArray(positionAttribHandle);
   glEnableVertexAttribArray(colorAttribHandle);
   glDrawArrays(GL_TRIANGLES, 0, 3);
   //__END_PROFILING__
}

