#include "Fontgenerator.h"
#include "FontFile/ACUTATR_TTF.h"
#include FT_STROKER_H
#include "Font.h"

namespace GLPi3D
{


FontGenerator::FontGenerator () : errorStatus(false) {
    if (FT_Init_FreeType(&library))
    {
        printf("Free type initialization failed");
        errorStatus = true;
        return;
    }
    
    atlasTex    = 0;
    texDimension= 0;
    squareSize  = 0;
}

FontGenerator::~FontGenerator () {
    FT_Done_Face(fontFace);
    FT_Done_FreeType(library);
    if(atlasTex) {
        glDeleteTextures(1, &atlasTex);
        atlasTex = 0;
    }
}

void FontGenerator::setPixel (GLubyte* texture, int offset, int size, int x, int y, GLubyte val) {
    texture[2*(offset+x+y*size)] = texture[2*(offset+x+y*size)+1] = val;
}

void FontGenerator::generateTexFromGlyph (FT_GlyphSlot glyph, GLubyte* texture, int atlasX, int atlasY, int texSize, int resolution, int marginSize, bool drawBorder) {

    const int squareSize = resolution + marginSize;
    const int baseOffset = atlasX*squareSize + atlasY*squareSize*texSize;
    
    if (drawBorder) {
        for (int w=0; w<squareSize; w++)
            { setPixel(texture, baseOffset, texSize, w, 0, 255); }
        
        for (int h=1; h<squareSize; h++)
            for (int w=0; w<squareSize; w++)
                setPixel(texture,baseOffset,texSize,w,h, (w==0||w==squareSize-1)?255:(h==squareSize-1)?255:0);
    }
    
    const int gr = glyph->bitmap.rows;
    const int gw = glyph->bitmap.width;
    for (int h=0; h<gr; h++) {
        for (int w=0; w<gw; w++) {
            setPixel(texture, baseOffset+marginSize, texSize, w, marginSize+h, glyph->bitmap.buffer[w+h*gw]);
        }
    }

    if (drawBorder) {
        return;
    }

    return;
    /*for (int h=0; h<squareSize; h++) {
        if (h < marginSize || h >= marginSize+gr) {
            for (int w=0; w<squareSize; w++)//bottom padding => whole rows
                { setPixel(texture, baseOffset, texSize, w, h, 0); }
        } else {
            for (int w=0; w<marginSize; w++) //left padding
                { setPixel(texture, baseOffset, texSize, w, h, 0); }
            for (int w=gw+marginSize; w<squareSize; w++) //right padding
                { setPixel(texture, baseOffset, texSize, w, h, 0); }
        }
    }*/
}

bool FontGenerator::errorState () {
    return errorStatus;
}

bool FontGenerator::readFont (const FT_Face& fontFace, int resolution, int glyphMargin) {

    FT_Set_Pixel_Sizes(fontFace, resolution, resolution);
    const int numGlyphs         = fontFace->num_glyphs;
    
    //The actual size of a side of the square that will contain our glyph
    squareSize                  = (float)(resolution + glyphMargin);
    
    //Calculate the size of the texture we'll need to store these glyphs
    const int numGlyphsPerRow   = (int)ceilf((float)sqrt((double)numGlyphs)); //=numRows (texture is a square)
    
    const int texSize           = (numGlyphsPerRow)*(int)squareSize;

    //OpenGL ES requires power of 2 textures
    int realTexSize             = GLUtils::nextPowerOf2(texSize);
    
    //we use two channels (one for luminance, one for alpha)
    GLubyte* textureData        = new GLubyte[realTexSize*realTexSize*2];
    memset(textureData,0,realTexSize*realTexSize*2);
    if (atlasTex){
        glDeleteTextures(1, &atlasTex);
        atlasTex = 0;
    }
    
    glGenTextures(1, &atlasTex);
    glBindTexture(GL_TEXTURE_2D, atlasTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    GLUtils::checkForOpenGLError(__FILE__, __LINE__);
    
    
    int texAtlasX       = 0;
    int texAtlasY       = 0;
    FT_UInt gindex      = 0;

    for (FT_ULong charcode=FT_Get_First_Char(fontFace, &gindex); gindex != 0;
             charcode=FT_Get_Next_Char(fontFace, charcode, &gindex)) {
        
        //gindex = myc;
        if (FT_Load_Glyph(fontFace, gindex, FT_LOAD_DEFAULT)) {
            printf("Error loading glyph with index %i and charcode %i. Skipping.", gindex, (int)charcode);
            continue;
        }
        
        FT_GlyphSlot glyph = fontFace->glyph;
        FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);
        
        //Calculate glyph informations
        Glyph glyphInfo;
        glyphInfo.metric		= glyph->metrics;
        
        // Get texture offset in the image
        glyphInfo.atlasX		= (texAtlasX*squareSize)/(float)realTexSize;
        glyphInfo.atlasY		= (texAtlasY*squareSize)/(float)realTexSize;
        
        //advance is stored in fractional pixel format (=1/64 pixels), as per free type specifications
        glyphInfo.advanceHorizontal	= (glyph->advance.x)/(float)(64.0f);
        glyphInfo.advanceVertical	= (glyph->advance.y)/(float)(64.0f);
        glyphInfo.charcode          = charcode;
        glyphs[charcode]        = glyphInfo;
        
        //Copy the bitmap to the atlas
        generateTexFromGlyph(glyph, textureData, texAtlasX, texAtlasY, realTexSize, resolution, glyphMargin, false);
        
        texAtlasX++;
        if (texAtlasX >= numGlyphsPerRow) {
            texAtlasX=0;
            texAtlasY++;
        }
    }
    //FILE* fp = fopen("D:\\Development\\GS4_WITH_GLF\\pkg\\graphic\\glpi3d\\core\\engine\\Font\\TextureFont.bin","wb");
    //if(fp)
    //{
    //   fwrite(textureData, sizeof(GLubyte)*realTexSize*realTexSize*2,1,fp);
    //   fclose(fp);
    //}
    //GLubyte* fileTextureData = NULL;
    //fp = fopen("D:\\Development\\GS4_WITH_GLF\\pkg\\graphic\\glpi3d\\core\\engine\\Font\\TextureFont.bin","rb");
    //if(fp)
    //{
    //   fseek(fp, 0L, SEEK_END);
    //   int sz = ftell(fp);
    //   fseek(fp, 0L, SEEK_SET);
    //   fileTextureData = new GLubyte[sz];
    //   fread(fileTextureData, sizeof(GLubyte)*sz,1,fp);
    //   fclose(fp);
    //}
    glTexImage2D (GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, realTexSize, realTexSize, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, textureData);
    
    delete [] textureData;
    //delete [] fileTextureData;

    //writeGlyphsToFile();
    //readGlyphsFromFile();
    //readGlyphsFromHeader();
    GLUtils::checkForOpenGLError(__FILE__, __LINE__);

    texDimension = (squareSize)/(float)realTexSize;
    return true;
}

bool FontGenerator::getCorrectResolution(const FT_Face& fontFace, int resolution, int& newResolution, int& newGlyphMargin)
{
    // Create the texture as per correct size specified by the OEM provider.
    int glyphMargin = (int)ceil(resolution*0.1f);
    while(true){
        const long numGlyphs        = fontFace->num_glyphs;
        
        const int squareSize        = resolution + glyphMargin;
        
        const int numGlyphsPerRow   = (int)ceilf((float)sqrt((double)numGlyphs));
        const int texSize           = (numGlyphsPerRow)*squareSize;
        int realTexSize             = GLUtils::nextPowerOf2(texSize);
        
        GLint MaxTextureSize;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &MaxTextureSize);
        if(realTexSize <= MaxTextureSize || resolution <= 0){
            break;
        }
        
        resolution  = resolution - 5;    //Decrease the resolution by 5 units.
        glyphMargin = (int)ceil(resolution*0.1f);
    }
    
    if(resolution > 0){
        newResolution   = resolution;
        newGlyphMargin  = glyphMargin;
        return true;
    }
    else{
        return false;
    }
}

bool FontGenerator::loadFont(const char* filename, int resolution) {
    // Check any error of appear in the initialization FTLibrary
    if (errorStatus) {
        printf("loadFont : freetype library initialization failed");
        return false;
    }
   
    // Generate the face object from File currently the file support is not supported in the "pkg\graphic\fnrft\freetype\config\ftstdlib.h"
    //if (FT_New_Face(library, filename, 0, &fontFace)) {
    //    printf("Error loading font face %s", filename);
    //    return false;
    //}

    //Generate the face object
    if (FT_New_Memory_Face(library, ACUTATR_TTF, sizeof(ACUTATR_TTF), 0, &fontFace)) {
        printf("Error loading font face %s", filename);
        return false;
    }

    // Get corect resolution from system and generate font bitmaps.
    int calculatedResoution; int calculatedMargin;
    if(getCorrectResolution(fontFace, resolution, calculatedResoution, calculatedMargin))
    {
        return readFont(fontFace, calculatedResoution, calculatedMargin);
    }
    return true;
}

bool FontGenerator::writeGlyphsToFile()
{
   FILE *ptr_myfile;

   ptr_myfile=fopen("pkg\\graphic\\grlc\\core\\OpenGLESFramework\\Font\\glyphs.bin","wb");
   if (!ptr_myfile)
   {
      printf("Unable to open file!");
      return false;
   }
   for (std::map<unsigned long, Glyph>::iterator it=glyphs.begin(); it!=glyphs.end(); ++it){
      fwrite(&it->second, sizeof(struct Glyph), 1, ptr_myfile);
   }
   fclose(ptr_myfile);
   return true;
}

bool FontGenerator::readGlyphsFromHeader()
{
   int glyphCount = sizeof(glyphs_bin)/sizeof(Glyph);
   Glyph* ptr = (Glyph*)glyphs_bin;
   glyphs.clear();
   for(int i=0;i<glyphCount;i++){
      Glyph glyphItem;
      memcpy(&glyphItem, ptr, sizeof(Glyph));
      glyphs[glyphItem.charcode]=glyphItem;
      ptr++;
   }
   return true;
}

bool FontGenerator::readGlyphsFromFile(char* path)
{
   FILE *ptr_myfile;
   glyphs.clear();
//"pkg\\graphic\\grlc\\core\\OpenGLESFramework\\Font\\glyphs.bin"
   ptr_myfile=fopen(path, "rb");
   if (!ptr_myfile)
   {
      printf("Unable to open file!");
      return false;
   }

   fseek(ptr_myfile, 0L, SEEK_END);
   int size = ftell(ptr_myfile);
   fseek(ptr_myfile, 0L, SEEK_SET);
   int glyphCount = 0;
   if(size>1){
      glyphCount = size/sizeof(Glyph);
   }
   for(int i=0;i<glyphCount;i++){
      Glyph glyphItem;
      fread(&glyphItem, sizeof(struct Glyph), 1, ptr_myfile);
      glyphs[glyphItem.charcode]=glyphItem;
   }
   fclose(ptr_myfile);
   return true;
}

bool FontGenerator::readFontFromBin (char* textureAtlasPath, char* glyphsDataStructPath)
{
    GLubyte* fileTextureData = NULL;
    FILE* fp = fopen(textureAtlasPath,"rb");
    double texSize = 0;
    if(fp)
    {
       fseek(fp, 0L, SEEK_END);
       int sz = ftell(fp);
       texSize = sqrt(texSize)/2;
       fseek(fp, 0L, SEEK_SET);
       fileTextureData = new GLubyte[sz];
       fread(fileTextureData, sizeof(GLubyte)*sz,1,fp);
       fclose(fp);
    }
    int realTexSize = (int)texSize;
    glTexImage2D (GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, realTexSize, realTexSize, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, fileTextureData);
    delete [] fileTextureData;
    readGlyphsFromFile(glyphsDataStructPath);
   return true;
}

bool FontGenerator::readFontFromHeader ()
{
   return true;
}
}

