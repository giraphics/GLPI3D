#include "glpi3dt_demotexquad.h"
#include "GLPi3D__Constant.h"

extern "C"
{
   #include "gbt_c1.h"
   #include "rsmg_c1.h" 
   #include "gosw_c1.h"
   #include "rsmg_c1.h"
   #include "rsst_c1.h"
   #include "grlc_c1.h"
   #include "cdef.h"
   #include "grlc_c1.h"
   #include "grlct_c1.h"
   #include "grlct_ci.h"
   extern GRLC__tstContext* GRLC__pstGetContext ( uint16 context_id );
   extern GBT_tstMSurface *RSMG_pstFillPipeMSrfByBmpID(RSMG_tstContext *pstContext, 
                RSST_tenBitmapID enBmpID,  bool boIsInTexCache, RSST_tenMemLocID enMemLocID);
}
#if SHADER_FILE_SUPPORT == 1
#define VERTEX_SHADER_PRG        (::int8 * )"pkg\\graphic\\glpi3d\\core\\samples\\shader\\TextureVertex.glsl"
#define FRAGMENT_SHADER_PRG      (::int8 * )"pkg\\graphic\\glpi3d\\core\\samples\\shader\\TextureFragment.glsl"
#else
#define VERTEX_SHADER_PRG        (::int8*)TextureVertex
#define FRAGMENT_SHADER_PRG      (::int8*)TextureFragment
#endif

static const GLchar TextureVertex[] = 
"                                            \
attribute vec4 VertexPosition;               \
attribute vec2 VertexTexCoord;               \
uniform mat4    ModelViewProjectionMatrix;   \
varying vec2 TexCoord;                       \
void main() {                                \
    TexCoord = VertexTexCoord;               \
    gl_Position = ModelViewProjectionMatrix * VertexPosition;            \
}                                            \
\0";

static const GLchar TextureFragment[] = 
"                                            \
precision mediump float;                     \
varying vec2 TexCoord;                       \
uniform sampler2D Tex1;                      \
void main() {                                \
gl_FragColor = texture2D(Tex1, TexCoord);      \
}                                            \
\0";

// Global Object Declaration
static GLfloat texCoords[8] = {  0.0f, 1.0f,
                                 1.0f, 1.0f,
                                 0.0f, 0.0f,
                                 1.0f, 0.0f
                              };

static GLfloat quad[12]     = {
                                -0.50f, -0.50f,  0.0f,
                                 0.50f, -0.50f,  0.0f,
                                -0.50f,  0.50f,  0.0f,
                                 0.50f,  0.50f,  0.0f
                              };

/*!
   Simple Default Constructor

   \param[in] None.
   \return None
*/
DemoTexQuad::DemoTexQuad( ModelType typeOfModel, Model* parent ):Model(typeOfModel, parent)
{
}


/*!
   Simple Destructor

   \param[in] None.
   \return None

*/
DemoTexQuad::~DemoTexQuad()
{
   tstProgram* program = NULL;
   if ( program = ProgramManagerObj->Program( (::int8* )"TexQuad" ) ){
      ProgramManagerObj->RemoveProgram(program);
   }
}

/*!
   Initialize the scene, reserve shaders, compile and chache program

   \param[in] None.
   \return None

*/
void DemoTexQuad::InitModel()
{
   if (!(program = ProgramManagerObj->Program( (::int8*) "TexQuad"))){
      program = ProgramManagerObj->ProgramInit( (::int8*) "TexQuad" );
      ProgramManagerObj->AddProgram( program );
   }

   program->VertexShader	= ShaderManager::ShaderInit( VERTEX_SHADER_PRG,	GL_VERTEX_SHADER);
   program->FragmentShader	= ShaderManager::ShaderInit( FRAGMENT_SHADER_PRG, GL_FRAGMENT_SHADER);

   if( !ProgramManagerObj->ProgramLink( program, 1 ) ) exit( 3 );

   glUseProgram( program->ProgramID );
   positionAttribHandle = ProgramManagerObj->ProgramGetVertexAttribLocation(program,(char*)"VertexPosition");
   texCoordAttribHandle = ProgramManagerObj->ProgramGetVertexAttribLocation(program, (char*)"VertexTexCoord");
   MVP                  = ProgramManagerObj->ProgramGetUniformLocation( program, ( char* )"ModelViewProjectionMatrix" );

   // Activate the texture unit 0
   // ROCH
   glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
   glGenTextures(1, &texture_id);
   glBindTexture(GL_TEXTURE_2D, texture_id);

   GRLC__tstContext *pstContext = GRLC__pstGetContext(sceneHandler->currentContext());
   RSST_tenBitmapID bmpID = RSST_nBMP_Logo_Mercedes;
   GBT_tstMSurface *pstMSurface = RSMG_pstFillPipeMSrfByBmpID( &pstContext->stRSMGCtxt, bmpID, 0, RSST_nMemJCP2011_DRAM /*GRLC_nDEFAULT_MEM_VRAM*/ ); 
   int32 w, h;
   if ( pstMSurface )
   {
      void *imgdata = pstMSurface->stSrfDesc.stBmp.apvPixBuffer[0];
      w = pstMSurface->stSrfDesc.stBmp.u16Width;
      h = pstMSurface->stSrfDesc.stBmp.u16Height;
      switch (pstMSurface->stSrfDesc.stBmp.u8PixFormatIdx)
      {
         case RSST_nPFrmt_A8:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, w, h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, imgdata);
         break;
         case RSST_nPFrmt_RGB565:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, imgdata);
         break;
         case RSST_nPFrmt_RGBA4444:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_SHORT_4_4_4_4, imgdata);
         break;
         case RSST_nPFrmt_RGB888:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, imgdata);
         break;
         case RSST_nPFrmt_ARGB8888:
         case RSST_nPFrmt_RGBA8888:
         default:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgdata);
         break;
      }
   }

   glActiveTexture (GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture_id);
   static bool a =true;
   a=!a;
   if (a){
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   }
   else{
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   }
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   TEX = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "Tex1" );

   glUniform1i(TEX, 0);
   return;
}

void DemoTexQuad::SetGLStates()
{
   glEnable (GL_BLEND);
   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glDisable(GL_CULL_FACE);
}

/*!
   Initialize the scene, reserve shaders, compile and chache program

   \param[in] None.
   \return None

*/
void DemoTexQuad::Render()
{
   //__START_PROFILING__

   glUseProgram( program->ProgramID );
   glActiveTexture (GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture_id);

   // GRLCDrawBMPCompare();

   static float angle = 0.0;
   //static float scale = 2.0;
   //static bool sf = true;
   //if (scale>=12.0){
   //   sf = false;
   //}
   //else{
   //   if(sf)
   //   scale += 0.1;
   //}
   //
   //if(!sf)
   //{
   //   if(GRLC_OGLRenderer::Instance()->frameCount() > 500){
   //      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   //      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   //   }
   //}
   //
   TransformObj->PushMatrix();
   TransformObj->Translate(1.5f, 0.6f, -1.0f);
   //TransformObj->Scale(scale, scale, scale);
   TransformObj->Rotate(angle += 0.4f, 0.0, 0.0, 1.0);
   glUniformMatrix4fv( MVP, 1, GL_FALSE,( float * )TransformObj->GetModelViewProjectionMatrix() );
   TransformObj->PopMatrix();

   glEnableVertexAttribArray(positionAttribHandle);
   glEnableVertexAttribArray(texCoordAttribHandle);
   glVertexAttribPointer(texCoordAttribHandle, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
   glVertexAttribPointer(positionAttribHandle, 3, GL_FLOAT, GL_FALSE, 0, quad);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
   //__END_PROFILING__
   return;
}

void DemoTexQuad::TextureFilteringTest()
{
   static bool a =!true;
   //a=!a;
   static int cnt = 0;
   if (/*cnt++>100*/a){
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   }
   else{
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   }
}

void DemoTexQuad::GRLCDrawBMPCompare()
{
   //__START_PROFILING__
   GRLC__tstContext *pstContext = GRLC__pstGetContext(sceneHandler->currentContext());
   //GRLC_vPrimSetCompMode(RenderHandler->currentContext(), GBT_nCompose_Overwrite);
   GRLC_vPrimSetBgColor(sceneHandler->currentContext(), 0x11220055, RSST_nPFrmt_RGBA8888);

   //GRLC_vClear(RenderHandler->currentContext());
   //GRLC_vPixSetCompMode(RenderHandler->currentContext(), GBT_nCompose_Overwrite);
   GRLC_vDrawBitmapID(sceneHandler->currentContext(), 400, 260, RSST_nBMP_Logo_Mercedes, NULL);
   //__END_PROFILING__

}


