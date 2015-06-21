#include "Font.h"

#if SHADER_FILE_SUPPORT == 1
#define VERTEX_SHADER_PRG        (::int8 * )"pkg\\graphic\\glpi3d\\core\\samples\\shader\\FontVertex.glsl"
#define FRAGMENT_SHADER_PRG      (::int8 * )"pkg\\graphic\\glpi3d\\core\\samples\\shader\\FontFragment.glsl"
#else
#define VERTEX_SHADER_PRG        (::int8*)FontVertex
#define FRAGMENT_SHADER_PRG      (::int8*)FontFragment
#endif

namespace GLPi3D
{

static const GLchar FontVertex[] = 
"                                            \
attribute vec3  VertexPosition;              \
attribute vec2  VertexTexCoord;              \
varying vec2 TexCoord;                       \
uniform mat4 ModelViewProjectMatrix;         \
void main( void ) {                          \
    TexCoord    = VertexTexCoord;            \
    gl_Position = ModelViewProjectMatrix * vec4(VertexPosition,1.0); \
}                                            \
\0";

static const GLchar FontFragment[] = 
"                                            \
precision mediump float;                     \
varying vec2 TexCoord;                       \
uniform sampler2D FontTexture;               \
uniform vec4 TextColor;                      \
void main() {                                \
    vec4 texcol = texture2D(FontTexture, TexCoord); \
    gl_FragColor = vec4(vec3(TextColor.r, TextColor.g, TextColor.b), texcol.a); \
}                                            \
\0";

Font::Font(char* fileName, int Size, ModelType LanguageType, Model* parent):Model(LanguageType, parent)
{
    loadFont(fileName, Size);
}

Font::~Font() {
	if ((program = ProgramManagerObj->Program((int8*) "font"))) {
		ProgramManagerObj->RemoveProgram(program);
	}
}

void Font::Render() {
}

void Font::InitModel() {
   if (! ( program = ProgramManagerObj->Program( (int8*)"font" ) ) ){
      program = ProgramManagerObj->ProgramInit( (int8*)"font" );
      ProgramManagerObj->AddProgram( program );
   }

   program->VertexShader	= ShaderManager::ShaderInit( VERTEX_SHADER_PRG,	GL_VERTEX_SHADER);
   program->FragmentShader	= ShaderManager::ShaderInit( FRAGMENT_SHADER_PRG, GL_FRAGMENT_SHADER);

   if (!ProgramManagerObj->ProgramLink(program, 1))
      exit(3);
   
   positionAttribHandle = ProgramManagerObj->ProgramGetVertexAttribLocation(program,(char*)"VertexPosition");
   texAttribHandle      = ProgramManagerObj->ProgramGetVertexAttribLocation(program,(char*)"VertexTexCoord");

   // Get the uniform location cache for optimization purpose
   MVP         = ProgramManagerObj->ProgramGetUniformLocation( program, (char*)"ModelViewProjectMatrix" );
   TEX         = ProgramManagerObj->ProgramGetUniformLocation( program, (char *) "FontTexture" );
   FRAG_COLOR  = ProgramManagerObj->ProgramGetUniformLocation( program, (char*)"TextColor" );
}

void Font::drawGlyph(const Glyph& gi) {
    glUseProgram(program->ProgramID);

	float xmargin = float(gi.metric.width)/(float) (2*64);
	float ymargin = float(gi.metric.horiBearingY)/(float)(2*64);
    
    // Calculate the texture coordinates for the glyph rendering.
    float texCoords[8] = {
        gi.atlasX, gi.atlasY,
        gi.atlasX + texDimension, gi.atlasY,
        gi.atlasX, gi.atlasY + texDimension,
        gi.atlasX + texDimension, gi.atlasY + texDimension
    };

    // Calculate the dimension of the glyph Quad.
    float quad[12]	= {
        -0.5f, 0.5f,  0.0f,
         0.5f, 0.5f,  0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f };

   for (int i = 0; i<12;){
      quad[i]   *= (float)squareSize/2;
      quad[i+1] *= (float)squareSize/2;
      quad[i+2] *= (float)0;
      i         += 3;
   }

   // Initialize the texture with texture unit 0
   glUniform1i(TEX, 1);
   TransformObj->PushMatrix();
   TransformObj->Translate(-xmargin, ymargin, 0.0f );
   glUniformMatrix4fv(MVP, 1, GL_FALSE, (float*)TransformObj->GetModelViewProjectionMatrix());
   TransformObj->PopMatrix();

   // Send the vertex and texture coordinates to the shaders
   glEnableVertexAttribArray(positionAttribHandle);
   glEnableVertexAttribArray(texAttribHandle);
   glVertexAttribPointer(positionAttribHandle, 3, GL_FLOAT, GL_FALSE, 0, quad);
   glVertexAttribPointer(texAttribHandle, 2, GL_FLOAT, GL_FALSE, 0, texCoords);
   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Font::plainText(char* str, GLfloat Red, GLfloat Green, GLfloat Blue, GLfloat Alpha) {
   // Initialize OpenGL ES States
   glDisable(GL_CULL_FACE);
   glDisable(GL_DEPTH_TEST);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   // Use font program
   glUseProgram(program->ProgramID);

   // Activate Texture unit 0 and assign the altas texture
   glActiveTexture (GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, atlasTex);

   TransformObj->PushMatrix();
   GLfloat color[4] = {Red, Green, Blue, Alpha};
   glUniform4fv(FRAG_COLOR, 1, color);

   for (const char* c = str; *c != '\0'; c++) {
   const Glyph& gi = glyphs[((unsigned long) *c)];
      TransformObj->Translate(gi.advanceHorizontal / 2.0f, 0.0, 0.0);
      drawGlyph(gi);
   }
   TransformObj->PopMatrix();
   return;
}

void Font::animateText(char* str, GLfloat Red, GLfloat Green, GLfloat Blue, GLfloat Alpha,float radius,float rot)
{
   // Initialize OpenGL ES States
   glDisable(GL_CULL_FACE);
   glDisable(GL_DEPTH_TEST);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   // Use font program
   glUseProgram(program->ProgramID);

   // Activate Texture unit 0 and assign the altas texture
   glActiveTexture (GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, atlasTex);

   TransformObj->PushMatrix();
   GLfloat color[4] = {Red, Green, Blue, Alpha};
   glUniform4fv(FRAG_COLOR, 1, color);

   int num_segments = strlen(str);
   int a = 0;
   float theta = 0;
   Glyph gi;
   for (const char* c = str; *c != '\0'; c++) {
      TransformObj->PushMatrix();
      TransformObj->Rotate(rot , 0.0, 1.0, 0.0);
      theta = 2.0f * 3.1415926f * float(a++) / float(num_segments);//get the current angle

      TransformObj->PushMatrix();
      TransformObj->Translate(radius * cosf(theta), 0.0, radius * sinf(theta));
      gi = glyphs[((unsigned long) *c)];
      TransformObj->Rotate(-rot , 0.0, 1.0, 0.0); // adjust 
      drawGlyph(gi);
      TransformObj->PopMatrix();
      TransformObj->PopMatrix();
   }
   TransformObj->PopMatrix();
   return;
}
}

