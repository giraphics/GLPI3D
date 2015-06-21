#ifndef ProfileData_H
#define ProfileData_H
#ifdef __cplusplus
#include "GLPi3D__Constant.h"
#include <time.h>
#include <vector>
#include <string>
using namespace std;

namespace GLPi3D
{


class ProfileData
{

public:
   //! Constructor QIProfiler.
    ProfileData();

    //! Parameterized Constructor QIProfiler.
   ProfileData(const std::string &function_name);
    
   //! Copy Constructor QIProfiler.
   ProfileData(const ProfileData &obj);
    
   //! Constructor QIProfiler.
   ~ProfileData();

   //! Overloaded Assignment operator.
    ProfileData& operator=(const ProfileData &obj);

   //! Helpful for debuging purpose.
    std::string debugString();
    
   //! This accumulate the time and populate the timeline.
    bool timeCall(long time_elasped);

   //! Give average time.
   double averageTime();
    
   //! Give Total Time.
   long totalTime();

   //! Vector base timeLine used for stack all processed time.
   std::vector<long> timeLine;

   //! Name of function.
    std::string functionName;

   //! Total number of calls made.
    unsigned long numberOfCalls;

   //! Processed time accumulation variable.
    long totalTimeElapsed;
};
#endif

}
#endif // ProfileData_H

