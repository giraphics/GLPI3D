#include "glpi3dt_demofontsample.h"
#include "Font/Font.h"
#include <time.h>

FontSample::FontSample( ModelType typeOfModel, Model* parent):Model(typeOfModel, parent)
{
   // Note: The below font path is not used, instead the font file is loaded from memory. See ACUTATR_TTF.h
   //sceneHandler->addModel( new Font("pkg\\graphic\\glpi3d\\core\\engine\\Font\\FontFile\\ACUTATR.TTF", 30, FontEnglish) );
}

FontSample::~FontSample()
{
}

void FontSample::Render()
{
   //__START_PROFILING__
   char buff[500];
   Font* English = dynamic_cast<Font*> (sceneHandler->getModel(FontEnglish));
   static float angle = 0.0;
   angle += 1.0;
   if(angle > 360){ angle = 0.0; }

   // current date/time based on current system
   time_t now = time(0);
   // convert now to string form
   char* dt = ctime(&now);
   // convert now to tm struct for UTC
   tm *gmtm = gmtime(&now);
   dt = asctime(gmtm);
   float scale = 2.5f/200.0f;

   //__START_PROFILING_CUSTOM__
   sprintf (buff,"%s", dt);
   TransformObj->PushMatrix();
   TransformObj->Translate(-1.70f, -1.0f, -1.0f);
   TransformObj->Rotate(angle, 1.0, 0.0, 0.0);
   TransformObj->Scale(scale, scale, scale);
   English->plainText((char*)buff, 0.60f, 0.80f, 0.80f, 1.0f );
   TransformObj->PopMatrix();
   //__END_PROFILING_CUSTOM__("DateTime")

   //__START_PROFILING_CUSTOM__
   TransformObj->PushMatrix();
   TransformObj->Translate(-1.0, 0.50, -1.0);
   TransformObj->Scale(scale, scale*2, scale);
   static float rot = 0.0;
   TransformObj->Rotate(15 , 1.0, 0.0, 0.0);
   English->animateText((char*)"Hello World !!! ", 1.0, 1.0, 0.0, 1.0f, 50.0f, rot+=0.4f);
   TransformObj->PopMatrix();
   //__END_PROFILING_CUSTOM__("Hello World")

   // show frames per second
   static int frames = 0;
   static clock_t time1 = clock();
   frames++;
   clock_t time2 = clock();
   if ((time2 - time1) >= CLOCKS_PER_SEC){
      sceneHandler->fps() = frames;
      frames   = 0;
      time1    = time2;
      //__FPS__(sceneHandler->fps());
   }

   memset(buff, 0, 500);
   //printf ("\nFPS: %d", RenderHandler->frameCount());
   sprintf (buff,"\nFPS: %d, FrameCount: %d", sceneHandler->fps(), sceneHandler->frameCount());

   //__START_PROFILING_CUSTOM__
   TransformObj->PushMatrix();
   TransformObj->Translate(-1.70f, 0.90f, -1.0f);
   TransformObj->Scale(scale, scale, scale);
   English->plainText((char*)buff, 1.0, 1.0, 1.0, 1.0f );
   TransformObj->PopMatrix();
   //__END_PROFILING_CUSTOM__("FPSRendering")
   //__END_PROFILING__
}

void FontSample::InitModel()
{
}

