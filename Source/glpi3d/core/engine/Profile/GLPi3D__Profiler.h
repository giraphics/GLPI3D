#ifndef Profiler_H
#define Profiler_H
#include "GLPi3D__Profiledata.h"
#ifdef __cplusplus
#include <map>
using namespace std;
namespace GLPi3D
{

class Profiler
{

public:
   //! Constructor Profiler.
   Profiler(){ profile= false; }

   //! Destructor Profiler.
   ~Profiler(){}

   //! Profile each function.
   void timeFunctionCall(const std::string &function_name, long elasped_time);

   //! Reset profiling.
   void resetFunctionProfiles();

   //! Generate the profile result in CVS format.
   void outputFunctionProfiles();

   //! Set the profiling with flag true or false.
   void setProfiling( bool flag );

   //! Profiling status flag.
   bool profiling(){return profile;}

private:
   //! Map for ProfileData type.
   std::map<std::string, ProfileData> runtimeProfile;

   //! Flag for enable and disable profiling.
   bool profile;
};
// HI_RESOLUTION APIS
#endif
#define HI_RESOLUTION_PROFILING 

#ifdef HI_RESOLUTION_PROFILING
   #include <windows.h>
   //HIGH RESOLUTION API's
   #ifdef __cplusplus //// IF DEF CPP 1
   // C++ Version of Profiling
   #define __START_PROFILING__	{\
   LARGE_INTEGER sTime; QueryPerformanceCounter(&sTime); \
   LARGE_INTEGER eTime;

   #define __END_PROFILING__ \
   QueryPerformanceCounter(&eTime); \
   sceneHandler->Profiler()->timeFunctionCall(__FUNCTION__, (long)eTime.QuadPart - (long)sTime.QuadPart);}

   #define __START_PROFILING_CUSTOM__	{\
   LARGE_INTEGER sCTime; QueryPerformanceCounter(&sCTime); \
   LARGE_INTEGER eCTime;

   #define __END_PROFILING_CUSTOM__(X) \
   QueryPerformanceCounter(&eCTime); \
   sceneHandler->Profiler()->timeFunctionCall(X, (long)eCTime.QuadPart - (long)sCTime.QuadPart);}

   #define __FPS__(X) {Scene::Instance()->Profiler()->timeFunctionCall("FPS", X);}
   #endif // ENDIF CPP 1

   #ifdef __cplusplus // IF DEF CPP 2
   extern "C" {
   #endif // ENDIF CPP 2

      void call__timeFunctionCall( char* fn, long elasped_time);
      void call_outputFunctionProfiles();

      // C Version of Profiling
      #define __START_PROFILING_C__	/*{\
      long sTime; QueryPerformanceCounter((LARGE_INTEGER *)&sTime);\
      long eTime;*/

      #define __END_PROFILING_C__ /*\
      QueryPerformanceCounter((LARGE_INTEGER *)&eTime);\
      Scene::Instance()->Profiler()->timeFunctionCall(__FUNCTION__, eTime - sTime);}*/

      #define __START_PROFILING_CUSTOM_C__   //{ long sCTime; QueryPerformanceCounter((LARGE_INTEGER *)&sCTime);
      

      #define __END_PROFILING_CUSTOM_C__(X)  //long eCTime; QueryPerformanceCounter((LARGE_INTEGER *)&eCTime); Scene::Instance()->Profiler()->timeFunctionCall(X, eCTime - sCTime);}

      #define __FPS_C__(X) {call__timeFunctionCall("FPS", X);}

   #ifdef __cplusplus // IF DEF CPP 3
   }
   #endif // ENDIF CPP 3
#else // ELSE OF HI_RESOLUTION_PROFILING
   //LOW RESOLUTION API's
   #include <time.h>
   #ifdef __cplusplus //// IF DEF CPP 4
   // C++ Version of Profiling
   #define __START_PROFILING__	{\
   clock_t sTime = clock(); \
   clock_t eTime;

   #define __END_PROFILING__ \
   eTime = clock(); \
   Scene::Instance()->Profiler()->timeFunctionCall(__FUNCTION__, eTime - sTime);}

   #define __START_PROFILING_CUSTOM__	{\
   clock_t sCTime = clock(); \
   clock_t eCTime;

   #define __END_PROFILING_CUSTOM__(X) \
   eCTime = clock(); \
   Scene::Instance()->Profiler()->timeFunctionCall(X, eCTime - sCTime);}

   #define __FPS__(X) {Scene::Instance()->Profiler()->timeFunctionCall("FPS", X);}
   #endif // ENDIF CPP 4

   #ifdef __cplusplus // IF DEF CPP 5
   extern "C" {
   #endif // ENDIF CPP 5

      void call__timeFunctionCall( char* fn, long elasped_time);
      void call_outputFunctionProfiles();

      // C Version of Profiling
      #define __START_PROFILING_C__	{\
      clock_t sTime = clock(); \
      clock_t eTime;

      #define __END_PROFILING_C__ \
      eTime = clock(); \
      call__timeFunctionCall(__FUNCTION__, eTime - sTime);}

      #define __START_PROFILING_CUSTOM_C__	{\
      clock_t sCTime = clock(); \
      clock_t eCTime;

      #define __END_PROFILING_CUSTOM_C__(X) \
      eCTime = clock(); \
      call__timeFunctionCall(X, eCTime - sCTime);}

      #define __FPS_C__(X) {call__timeFunctionCall("FPS", X);}

   #ifdef __cplusplus // IF DEF CPP 6
   }
   #endif // ENDIF CPP 6

#endif // ENDIF OF HI_RESOLUTION_PROFILING

//#ifdef __cplusplus
//// C++ Version of Profiling
//#define __START_PROFILING__	{\
//clock_t sTime = clock(); \
//clock_t eTime;
//
//#define __END_PROFILING__ \
//eTime = clock(); \
//Scene::Instance()->Profiler()->timeFunctionCall(__FUNCTION__, eTime - sTime);}
//
//#define __START_PROFILING_CUSTOM__	{\
//clock_t sCTime = clock(); \
//clock_t eCTime;
//
//#define __END_PROFILING_CUSTOM__(X) \
//eCTime = clock(); \
//Scene::Instance()->Profiler()->timeFunctionCall(X, eCTime - sCTime);}
//
//#define __FPS__(X) {Scene::Instance()->Profiler()->timeFunctionCall("FPS", X);}
//#endif
//
//#ifdef __cplusplus
//extern "C" {
//#endif
//
//   void call__timeFunctionCall( char* fn, long elasped_time);
//   void call_outputFunctionProfiles();
//
//   // C Version of Profiling
//   #define __START_PROFILING_C__	{\
//   clock_t sTime = clock(); \
//   clock_t eTime;
//
//   #define __END_PROFILING_C__ \
//   eTime = clock(); \
//   call__timeFunctionCall(__FUNCTION__, eTime - sTime);}
//
//   #define __START_PROFILING_CUSTOM_C__	{\
//   clock_t sCTime = clock(); \
//   clock_t eCTime;
//
//   #define __END_PROFILING_CUSTOM_C__(X) \
//   eCTime = clock(); \
//   call__timeFunctionCall(X, eCTime - sCTime);}
//
//   #define __FPS_C__(X) {call__timeFunctionCall("FPS", X);}
//
//#ifdef __cplusplus
//}
//#endif
}

#endif // Profiler_H

