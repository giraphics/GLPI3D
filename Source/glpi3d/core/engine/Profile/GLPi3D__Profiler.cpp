#include "GLPi3D__Profiler.h"
#include "GLPi3D__ProfileData.h"
#include "GLPi3D__Renderer.h"

#define ROOT_PATH "pkg\\graphic\\grlc\\core\\engine\\Profile\\"

namespace GLPi3D
{

#define FILE_OUTPUT false
#define FPS_TIMELINE true

/*!
   This function is responsible for updating or adding new ProfileData element.
   If it finds the element in the map it update its info otherwise insert a new element into map.

   \param[in] function_name Name of the function or any arbitary string that is used as profile marker.
   \param[in] elasped_time The time spent in processing this function or profile marker.

*/
void Profiler::timeFunctionCall(const std::string &function_name, long elasped_time)
{
   profile = true; //Hardcoded for profiling to be on
   if ( !profile ){
      return;
   }
   if ( runtimeProfile.count( function_name ) > 0 ) {
      if(!runtimeProfile.find( function_name )->second.timeCall( elasped_time )){
         resetFunctionProfiles();
      }
   }
   else {
     ProfileData fp( function_name );
     if(!fp.timeCall( elasped_time )){
         resetFunctionProfiles();
     }
     runtimeProfile[ function_name ] = fp;
   }
}

/*!
   This function reset the profile class.

   \param[in] None

*/
void Profiler::resetFunctionProfiles()
{
   runtimeProfile.clear();
}

/*!
   This function generates two files for profiling.
   1. ROOT_PATH/profile.csv
   This creates a profiling file which containing the section we want to profile.

   2. ROOT_PATH/FPSprofile.csv
   This creates a timeline base FPS profiler which let us know which frame the FPS drops.
   It can be used for to see the graphs view for FPS analysis.

   \param[in] None.

*/
void Profiler::outputFunctionProfiles()
{
   if ( !profile ){
      return;
   }
   char sProfileFullFilePathStr[MAX_PATH_LEN];
   sprintf(sProfileFullFilePathStr, "%s%s" , ROOT_PATH, "\\FunctionProfile.csv");

   char sFpsProfileFullFilePathStr[MAX_PATH_LEN];
   sprintf(sFpsProfileFullFilePathStr, "%s%s" , ROOT_PATH, "\\FPSprofile.csv");

   FILE *f        = NULL;
   char* filename = NULL;
   if( FILE_OUTPUT == true ){
      filename = (char*) sProfileFullFilePathStr;
      f = fopen(filename, "wt");
      if (!f) {
         printf("Unable to get the write access on:%s",sProfileFullFilePathStr);
         return;
      }
   }
   char caTempBuffer[500];
   memset(caTempBuffer,0,sizeof(caTempBuffer));
   // Format of log file: Function,Times called,Average duration(ms),Total duration(ms)
//LARGE_INTEGER frequency;QueryPerformanceFrequency(&frequency);
//sprintf(caTempBuffer,"%s: %ld\n","Resolution CLOCKS PER SEC",frequency.QuadPart);
   if( FILE_OUTPUT == true ){
      fprintf( f, caTempBuffer );
   }
   else{
      printf( caTempBuffer );
   }
   memset(caTempBuffer,0,sizeof(caTempBuffer));

   // Format of log file: Function,Times called,Average duration(ms),Total duration(ms)
   sprintf(caTempBuffer,"%s","\nFunction,      Times called,       Average duration,     Total duration\n");
   if( FILE_OUTPUT == true ){
      fprintf( f, caTempBuffer );
   }
   else{
      printf( caTempBuffer );
   }

   std::map<std::string, ProfileData>::iterator it;
   for ( it=runtimeProfile.begin() ; it != runtimeProfile.end(); it++ )
   {
      ProfileData curr = (*it).second;
      memset( caTempBuffer, 0, sizeof( caTempBuffer ) );

      //sprintf( caTempBuffer, "\n%-30s,%-10ld,%-10.8lf,%-8.8lf", curr.functionName.c_str(), curr.numberOfCalls, curr.averageTime()*1000/frequency.QuadPart, (float)curr.totalTime()*1000/frequency.QuadPart );
      if( FILE_OUTPUT == true ){
         fprintf( f, caTempBuffer );
      }
      else{
         printf( caTempBuffer );
      }
   }

   if( FILE_OUTPUT == true ){
      fclose(f);
   }
   
   if (FPS_TIMELINE == false ){
      return;
   }

   //Parminder: This creates a timeline base FPS profiler which let us know which frame the FPS drops.
   // It can be used for to see the graphs view for FPS analysis.
   if( FILE_OUTPUT == true ){
      filename = (char*) sFpsProfileFullFilePathStr;
      f = fopen(filename, "wt");
      if (!f) {
         printf("Unable to get the write access on:%s",sFpsProfileFullFilePathStr);
         return;
      }
   }

   printf("\n\n\n\n");
   memset(caTempBuffer,0,sizeof(caTempBuffer));
   sprintf(caTempBuffer,"%s","\nFrameNo,FPS");

   if( FILE_OUTPUT == true ){
      fprintf(f, caTempBuffer);
   }
   else{
      printf( caTempBuffer );
   }

   if ( runtimeProfile.count( "FPS" ) > 0 ) {
      ProfileData& profile = runtimeProfile.find( "FPS" )->second;
      unsigned int size = profile.timeLine.size();
      for (unsigned int i=0; i<size; i++){
         sprintf(caTempBuffer,"\n%d,%ld", i+1, profile.timeLine.at(i));
         if( FILE_OUTPUT == true ){
            fprintf(f, caTempBuffer);
         }
         else{
            printf( caTempBuffer );
         }
      }
   }

   if( FILE_OUTPUT == true ){
      fclose(f);
   }
}

void Profiler::setProfiling( bool flag ) 
{ 
   profile = flag; 
   if ( profile )
      runtimeProfile.clear();
}

Profiler profilerForC_call;
void call__timeFunctionCall( char* fn, long elasped_time) // wrapper function
{
   const std::string function_name(fn);
   profilerForC_call.timeFunctionCall(function_name, elasped_time);
}

void call_outputFunctionProfiles() // wrapper function
{
   profilerForC_call.outputFunctionProfiles();
}
}

