#include "glpi3dt_demorectangle.h"
#include "GLPi3D__Constant.h"

#if SHADER_FILE_SUPPORT == 1
#define VERTEX_SHADER_PRG        (int8 * )"pkg\\graphic\\glpi3d\\core\\samples\\shader\\TriangleVertex.glsl"
#define FRAGMENT_SHADER_PRG      (int8 * )"pkg\\graphic\\glpi3d\\core\\samples\\shader\\TriangleFragment.glsl"
#else
#define VERTEX_SHADER_PRG        (int8*)rectangleVertex
#define FRAGMENT_SHADER_PRG      (int8*)rectangleFragment
#endif

// Namespace used
//static GLfloat gRectangleVertices[]  = { 0.0f, 0.1f, -0.1f, -0.1f, 0.1f, -0.1f };
//static GLfloat gRectangleVertices[]  = { 0.0f, 480.0f, 100.0f, 380.0f, 100.0f, 480.0f };
static GLfloat gRectangleVertices[]  = { 50.0f, 50.0f, 100.0f, 100.0f, 100.0f, 0.0f };
static GLfloat gRectangleColors[]    = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };

// Global Object Declaration
const GLchar rectangleVertex[] =
"                                                           \
attribute vec4 VertexPosition;                              \
attribute  vec4 VertexColor;                                \
uniform float  RadianAngle;                                 \
varying vec4       TriangleColor;                           \
uniform mat4    ModelViewProjectionMatrix;                  \
void main() {                                               \
    vec4 V = VertexPosition; V.y = 480.0 - V.y;             \
    gl_Position =  ModelViewProjectionMatrix*V;             \
    /*gl_Position =  VertexPosition;*/                          \
    TriangleColor = VertexColor;                            \
}                                                           \
\0";

const GLchar rectangleFragment[] =
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
DemoRectangle::DemoRectangle( ModelType typeOfModel, Model* parent ):Model(typeOfModel, parent)
{
   degree = 0;
}


/*!
	Simple Destructor

	\param[in] None.
	\return None

*/
DemoRectangle::~DemoRectangle()
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
void DemoRectangle::InitModel()
{
   if (!(program = ProgramManagerObj->Program( (::int8*) "Triangle"))){
      program = ProgramManagerObj->ProgramInit( (::int8*) "Triangle" );
      ProgramManagerObj->AddProgram( program );
   }

   program->VertexShader   = ShaderManager::ShaderInit( VERTEX_SHADER_PRG, GL_VERTEX_SHADER );
   program->FragmentShader = ShaderManager::ShaderInit( FRAGMENT_SHADER_PRG, GL_FRAGMENT_SHADER);

   if( !ProgramManagerObj->ProgramLink( program, 1 ) ) exit( 3 );

   glUseProgram( program->ProgramID );
   MVP                  = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"ModelViewProjectionMatrix" );
   positionAttribHandle = ProgramManagerObj->ProgramGetVertexAttribLocation(program,(char*)"VertexPosition");
   colorAttribHandle    = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"VertexColor");

   return;
}

/*!
   Initialize the scene, reserve shaders, compile and chache program

   \param[in] None.
   \return None

*/
void DemoRectangle::Render()
{
   glUseProgram( program->ProgramID );

   radian = degree += 0.2f/57.2957795f;
   // Query and send the uniform variable.
   radianAngle = glGetUniformLocation(program->ProgramID, "RadianAngle");
   glUniform1f(radianAngle, radian);
   float x = 5.0; float y = 5.0; float width = 5.0; float height = 5.0;
   //__START_PROFILING__
   glUniformMatrix4fv( MVP, 1, GL_FALSE,( float * )TransformObj->GetProjectionMatrix() );
   for(int i=0;i<50000;i++){
      //width  = width;
      //height = height;
      //x        = (float)(2 * x) / width;
      //y        = (float)(2 * y) / height;
      //width    = (float)(2 * width) / width;
      //height   = (float)(2 * height) / height;
      //gRectangleVertices[0] = gRectangleVertices[0] *x/x;
      //gRectangleVertices[1] = gRectangleVertices[1] *x/x;
      //gRectangleVertices[2] = gRectangleVertices[2] *x/x;
      //gRectangleVertices[3] = gRectangleVertices[3] *x/x;
      //gRectangleVertices[4] = gRectangleVertices[4] *x/x;
      //gRectangleVertices[5] = gRectangleVertices[5] *x/x;
      glVertexAttribPointer(positionAttribHandle, 2, GL_FLOAT, GL_FALSE, 0, gRectangleVertices);
      glVertexAttribPointer(colorAttribHandle, 3, GL_FLOAT, GL_FALSE, 0, gRectangleColors);
      glEnableVertexAttribArray(positionAttribHandle);
      glEnableVertexAttribArray(colorAttribHandle);
      glDrawArrays(GL_TRIANGLES, 0, 3);
   }
   //__END_PROFILING__
}

