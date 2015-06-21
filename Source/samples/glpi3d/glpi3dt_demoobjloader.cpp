#include "glpi3dt_demoobjloader.h"

#include "./model/cube.h"
#include "./model/suzzane.h"
#include "./model/mercedesBenzClass.h"

using namespace glm;

#if SHADER_FILE_SUPPORT == 1
#define VERTEX_SHADER_PRG        (::int8 * )"pkg\\graphic\\glpi3d\\core\\samples\\shader\\DiffuseVertex.glsl"
#define FRAGMENT_SHADER_PRG      (::int8 * )"pkg\\graphic\\glpi3d\\core\\samples\\shader\\DiffuseFragment.glsl"
#else
#define VERTEX_SHADER_PRG        (::int8*)objVertex
#define FRAGMENT_SHADER_PRG      (::int8*)objFragment
#endif

#define USE_VAO 0

static const GLchar objVertex[] =
"                                                                          \
/* Vertex information */                                                   \
attribute vec3  Normal;                                                    \
attribute vec4  VertexPosition;                                            \
                                                                           \
uniform vec3    LightDiffuse;                                              \
uniform vec3    MaterialDiffuse;                                           \
uniform vec3    LightPosition;                                             \
                                                                           \
uniform mat4    ModelViewProjectionMatrix;                                 \
uniform mat4    ModelViewMatrix;                                           \
uniform mat3    NormalMatrix;                                              \
                                                                           \
varying vec4    FinalColor;                                                \
                                                                           \
void main()                                                                \
{                                                                          \
    vec3 nNormal   = normalize ( NormalMatrix * Normal );                  \
    vec3 eyeCoord  = vec3 ( ModelViewMatrix * VertexPosition );            \
    vec3 nLight    = normalize( LightPosition - eyeCoord );                \
                                                                           \
    /* Calculate cosine between Normal and Light vector */                 \
    float cosAngle = max( 0.0, dot( nNormal, nLight ));                    \
                                                                           \
    /* Compute diffuse Light and Material interaction */                   \
    vec3 diffuse   = MaterialDiffuse  * LightDiffuse;                      \
                                                                           \
    /* Calculate net intensity */                                          \
    vec3 Color 	   = cosAngle * diffuse;                                 \
    FinalColor 	   = vec4(Color, 1.0);                                   \
    gl_Position    = ModelViewProjectionMatrix * VertexPosition;           \
    gl_PointSize   = 15.0;     \
}                                                                          \
\0";

static const GLchar objFragment[] =
"                                                                          \
precision mediump float;                                                   \
varying vec4        FinalColor;                                            \
void main() {                                                              \
    gl_FragColor = FinalColor;                                             \
}                                                                          \
\0";


// Model Name Array
char ModelNames[][MAX_PATH_LEN]= {""};
/*!
   Simple Default Constructor

   \param[in] None.
   \return None

*/
ObjLoader::ObjLoader( ModelType typeOfModel, Model* parent ):Model(typeOfModel, parent)
{
   RenderPrimitive   = 3;
   ModelNumber       = 0;
   glEnable( GL_DEPTH_TEST );
   glDisable( GL_CULL_FACE );
}

void ObjLoader::ReleaseMeshResources()
{
   #if USE_VAO == 1
      glDeleteVertexArrays(1, &OBJ_VAO_Id);
   #else
   #endif
    glDeleteBuffers(1, &vertexBuffer);
}

void ObjLoader::SwitchMesh()
{
    // Release the old resources
    ReleaseMeshResources();

    // Get the new mesh model number
    ModelNumber++;
    ModelNumber %= sizeof(ModelNames)/(sizeof(char)*MAX_PATH_LEN);
}

void ObjLoader::LoadMeshFromHeader()
{
   stride          = 0;
   offset          = ( GLvoid*) ( sizeof(GLfloat)*3 * mbclassNumVerts);

   // Create the VBO for our obj model vertices.
   glGenBuffers(1, &vertexBuffer);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3 * mbclassNumVerts * 2, &mbclassVerts[0], GL_STATIC_DRAW);

   // Create the VAO, this will store the vertex attributes into vectore state.
   #if USE_VAO == 1
   glGenVertexArrays(1, &OBJ_VAO_Id);
   glBindVertexArray(OBJ_VAO_Id);
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
   glEnableVertexAttribArray(positionAttribHandle);
   glEnableVertexAttribArray(normalAttribHandle);
   glVertexAttribPointer(positionAttribHandle, 3, GL_FLOAT, GL_FALSE, stride, 0);
   glVertexAttribPointer(normalAttribHandle, 3, GL_FLOAT, GL_FALSE, stride, offset);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   glBindVertexArray(0);
   #endif
   IndexCount = mbclassNumVerts;
}

//void ObjLoader::LoadMesh()
//{
//   char fname[MAX_PATH]= {""};
//   strcpy( fname, "pkg\\graphic\\grlc\\core\\OpenGLESSample\\model\\" );
//   ModelNumber = 0;
//   strcat( fname, ModelNames[ModelNumber]);
//
//   objMeshModel    = waveFrontObjectModel.ParseObjModel(fname);
//   IndexCount      = waveFrontObjectModel.IndexTotal();
//   stride          = (2 * sizeof(vec3) )+ sizeof(vec2) + sizeof(vec4);
//   offset          = ( GLvoid*) ( sizeof(glm::vec3) + sizeof(vec2) );
//   offsetTexCoord  = ( GLvoid*) ( sizeof(glm::vec3) );
//
//   // Create the VBO for our obj model vertices.
//   glGenBuffers(1, &vertexBuffer);
//   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//   glBufferData(GL_ARRAY_BUFFER, objMeshModel->vertices.size() * sizeof(objMeshModel->vertices[0]), &objMeshModel->vertices[0], GL_STATIC_DRAW);
//
//   // Create the VAO, this will store the vertex attributes into vectore state.
//   //glGenVertexArrays(1, &OBJ_VAO_Id);
//   //glBindVertexArray(OBJ_VAO_Id);
//   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
//   glEnableVertexAttribArray(positionAttribHandle);
//   glEnableVertexAttribArray(normalAttribHandle);
//   glVertexAttribPointer(positionAttribHandle, 3, GL_FLOAT, GL_FALSE, stride, 0);
//   glVertexAttribPointer(normalAttribHandle, 3, GL_FLOAT, GL_FALSE, stride, offset);
//   glBindBuffer(GL_ARRAY_BUFFER, 0);
//   //glBindVertexArray(0);
//   objMeshModel->vertices.clear();
//}
/*!
   Simple Destructor

   \param[in] None.
   \return None

*/
ObjLoader::~ObjLoader()
{
   tstProgram* program = NULL;
   if ( program = ProgramManagerObj->Program(  (::int8* )"ShaderObj" ) )
   {
      ProgramManagerObj->RemoveProgram(program);
   }
}

/*!
   Initialize the scene, reserve shaders, compile and chache program

   \param[in] None.
   \return None

*/
void ObjLoader::InitModel()
{
   if (! ( program = ProgramManagerObj->Program( ( ::int8* )"ShaderObj" ) ) )
   {
      program = ProgramManagerObj->ProgramInit( ( ::int8* )"ShaderObj" );
      ProgramManagerObj->AddProgram( program );
   }

   program->VertexShader	= ShaderManager::ShaderInit( VERTEX_SHADER_PRG,	GL_VERTEX_SHADER	);
   program->FragmentShader	= ShaderManager::ShaderInit( FRAGMENT_SHADER_PRG, GL_FRAGMENT_SHADER	);

   if( !ProgramManagerObj->ProgramLink( program, 1 ) ) exit( 3 );

   glUseProgram( program->ProgramID );

   positionAttribHandle   = ProgramManagerObj->ProgramGetVertexAttribLocation(program,(char*)"VertexPosition");
   normalAttribHandle     = ProgramManagerObj->ProgramGetVertexAttribLocation(program,(char*)"Normal");
   GLint LightColor        = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"LightDiffuse");

   glm::vec3 color = glm::vec3(1.0, 1.0, 1.0);
   if (LightColor >= 0){
      glUniform3f(LightColor, color.r, color.g, color.b);
   }

   MaterialColor  = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"MaterialDiffuse");
   color = glm::vec3(material.r, material.g, material.b);
   if (MaterialColor >= 0){
      glUniform3f(MaterialColor, color.r, color.g, color.b);
   }

   LightPos  = ProgramManagerObj->ProgramGetUniformLocation(program, (char*)"LightPosition");
   if (LightPos >= 0){
      glUniform3f(LightPos, 0.0, 5.0, 3.0);
   }

   MVP           = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"ModelViewProjectionMatrix" );
   MV            = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"ModelViewMatrix" );
   NormalMatrix  = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"NormalMatrix" );

   // Load the new mesh model
   //LoadMesh();
   LoadMeshFromHeader();
   Model::InitModel();
   return;
}

/*!
   Initialize the scene, reserve shaders, compile and chache program

   \param[in] None.
   \return None

*/
void ObjLoader::Render()
{
   glUseProgram(program->ProgramID);

   if (MaterialColor >= 0){
      glUniform3f(MaterialColor, material.r, material.g, material.b);
   }

   TransformObj->PushMatrix();
   DrawDiffuseObject();
   for(unsigned int i=0;i<this->GetChildren()->size();i++){
      GetChildren()->at(i)->Render();
   }
   TransformObj->PopMatrix();

return;
   //QuatRot();
   //return;
      //__START_PROFILING__

   static int ii = 0;
   if(ii++>100) ii=0;float radius = 50;
   float theta = 2.0f * 3.1415926f * float(ii) / float(100);
   float x = radius * cosf(theta);
   float y = radius * sinf(theta);

   // Use Ambient program
   glUseProgram(program->ProgramID);
   //glUniform3f(LightPos,  x, 3.0, y);
   //__START_PROFILING_CUSTOM__
   CircularArrangement();
   //__END_PROFILING_CUSTOM__("CircularArrangement")
   TransformObj->PushMatrix();
   static float distanceZ = 50;
   static float distanceX = -25;
   if(distanceZ>0.0)
      distanceZ -= 0.025f;
   if(distanceX<25.0)
      distanceX += 0.025f;
   TransformObj->Translate(distanceX, 0.0f, distanceZ);
   DrawCubeOfObjects();
   TransformObj->PopMatrix();

   TransformObj->PushMatrix();
   TransformObj->Translate(0.0,-0.5, -0.5);
   static float angle = 0.0;
   TransformObj->Rotate(angle+= 0.4f , 0.0f, 1.0f, 0.0f);
   TransformObj->Rotate(-90.0f , 1.0f, 0.0f, 0.0f);
   DrawDiffuseObject();
   TransformObj->PopMatrix();
   //__END_PROFILING__
}

void ObjLoader::QuatRot()
{
	TransformObj->PushMatrix();
	//TransformObj->Translate(0.0,-0.5, -0.5);
	static float angle = 0.0;
	glm::quat a(1.0,1.0, 1.0, 1.0);
	//glm::detail::angleAxis(float(1.0),vec3(1.0,1.0,1.0));
	glm::vec3 v(1, 0, 0);
	glm::vec3 u(0, 1, 0);
	
	glm::quat Rotation = glm::rotation(v, u);
	
	float Angle = glm::angle(Rotation);
	
	int Error = glm::abs(Angle - glm::pi<float>() * 0.5f) < glm::epsilon<float>() ? 0 : 1;
	/////////////////////////////////////////////////////////////////////////////
	glm::quat A = glm::angleAxis(0.0f, glm::vec3(0, 0, 1));
	glm::quat B = glm::angleAxis(glm::pi<float>() * 0.5f, glm::vec3(0, 0, 1));
	glm::quat C = glm::shortMix(A, B, 0.5f);
	glm::quat D = glm::angleAxis(glm::pi<float>() * 0.25f, glm::vec3(0, 0, 1));
	
	//Error += glm::epsilonEqual(C.x, D.x, 0.01f) ? 0 : 1;
	//Error += glm::epsilonEqual(C.y, D.y, 0.01f) ? 0 : 1;
	//Error += glm::epsilonEqual(C.z, D.z, 0.01f) ? 0 : 1;
	//Error += glm::epsilonEqual(C.w, D.w, 0.01f) ? 0 : 1;
	//////////////////////////////////////////////////////////////////////////////
	glm::quat q(1.0f, 0.0f, 0.0f, 1.0f);
	float p = glm::roll(q);
	
	glm::quat q1(1.0f, 0.0f, 0.0f, 1.0f);
	float p1 = glm::pitch(q1);
	
	glm::quat q2(1.0f, 0.0f, 0.0f, 1.0f);
	float p2 = glm::yaw(q2);
	
	//TransformObj->Rotate(angle+= 0.4f , 0.0f, 1.0f, 0.0f);
	//TransformObj->Rotate(-90.0f , 1.0f, 0.0f, 0.0f);
	static float rot =0.0;
	rot += 0.1f;
	vec3 EulerAngles(-45, -45, 0);
	glm::quat za = glm::angleAxis(rot, glm::vec3(1, 1, -1));
	glm::quat za1 = quat(EulerAngles);
	//za1.w = rot/57.2;
	*(TransformObj->GetModelMatrix()) = glm::toMat4(za);
	DrawDiffuseObject();
	TransformObj->PopMatrix();
}

void ObjLoader::CircularArrangement()
{
   int num_segments = 10;
   float radius = 3.0;
   TransformObj->PushMatrix();
   TransformObj->Translate(0,0, -15);
   static float rot = 0.0;
   TransformObj->Rotate(45 , 1.0, 0.0, 0.0);
   TransformObj->Rotate(rot+= 0.1f , 0.0, 1.0, 0.0);
   for(int ii = 0; ii < num_segments; ii++)
   {
     float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle

     float x = radius * cosf(theta);//calculate the x component
     float y = radius * sinf(theta);//calculate the y component
     TransformObj->PushMatrix();
     TransformObj->Translate(x, 0.0, y);
     DrawDiffuseObject();
     TransformObj->PopMatrix();
   }
   TransformObj->PopMatrix();
}

void ObjLoader::DrawCubeOfObjects()
{
   static int dimension    = 10;
   static float distance   = 2.0;
   TransformObj->PushMatrix();
   TransformObj->Translate(0,0, -50);
   static float rot = 0.0;
   TransformObj->Rotate(rot+= 0.1f , 1.0, 1.0, 1.0);
   TransformObj->Translate(-distance*dimension/2,  -distance*dimension/2, -distance*dimension/2);
   for (int i = 0; i<dimension; i++){
      TransformObj->Translate(distance,  0.0, 0.0);
      TransformObj->PushMatrix();
      for (int j = 0; j<dimension; j++){
         TransformObj->Translate(0.0,  distance, 0.0);
         TransformObj->PushMatrix();
         for (int j = 0; j<dimension; j++){
            TransformObj->Translate(0.0,  0.0, distance);
            DrawDiffuseObject();
         }
         TransformObj->PopMatrix();
      }
      TransformObj->PopMatrix();
   }
   TransformObj->PopMatrix();
}

void ObjLoader::DrawDiffuseObject()
{
   // Apply Transformation.
   static float scale = 1.0;
   scale+= 0.01f;
   //TransformObj->PushMatrix();
   *TransformObj->GetModelMatrix() = *TransformObj->GetModelMatrix()*transformation;
   static float rot = 0.0;
   //TransformObj->Rotate(90, 0.0, 1.0, 1.0);
   glm::mat4 matrix    = *(TransformObj->GetModelViewMatrix());
   glm::mat3 normalMat = glm::mat3( glm::vec3(matrix[0]), glm::vec3(matrix[1]), glm::vec3(matrix[2]) );
   glUniformMatrix3fv( NormalMatrix, 1, GL_FALSE, (float*)&normalMat );
   glUniformMatrix4fv( MV, 1, GL_FALSE,( float * )TransformObj->GetModelViewMatrix() );
   glUniformMatrix4fv( MVP, 1, GL_FALSE,( float * )TransformObj->GetModelViewProjectionMatrix() );
   //TransformObj->PopMatrix();

   // Bind with Vertex Array Object for OBJ
   #if USE_VAO == 1
   glBindVertexArray(OBJ_VAO_Id);
   #else
   glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
   glEnableVertexAttribArray(positionAttribHandle);
   glEnableVertexAttribArray(normalAttribHandle);
   glVertexAttribPointer(positionAttribHandle, 3, GL_FLOAT, GL_FALSE, stride, 0);
   glVertexAttribPointer(normalAttribHandle, 3, GL_FLOAT, GL_FALSE, stride, offset);
   glBindBuffer(GL_ARRAY_BUFFER, 0);
   #endif


   RenderPrimitive = 2;
   // Draw Geometry
   if ( RenderPrimitive == 0 ){
      glDrawArrays(GL_POINTS, 0, IndexCount );
   }
   else if ( RenderPrimitive == 1 ){
      glDrawArrays(GL_LINE_LOOP, 0, IndexCount );
   }
   else{
      glDrawArrays(GL_TRIANGLES, 0, IndexCount );
   }

   #if USE_VAO == 1
   glBindVertexArray(0);
   #endif
   glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ObjLoader::SetGLStates()
{
   glEnable( GL_CULL_FACE );
   //glCullFace( GL_FRONT );
}

