#include "GLPi3D__ProfileData.h"
namespace GLPi3D
{

/*!
	Constructor ProfileData.

	\param[in] None.
*/
ProfileData::ProfileData() {
}

/*!
	Parameterized Constructor ProfileData.

	\param[in] function_name name of the function.
*/
ProfileData::ProfileData(const std::string &function_name) {
    functionName = function_name;
    numberOfCalls = 0;
    totalTimeElapsed = 0;
}

/*!
	Copy Constructor ProfileData.

	\param[in] obj ProfileData passed by reference.
*/
ProfileData::ProfileData(const ProfileData &obj) {
    timeLine         = obj.timeLine;
    functionName     = obj.functionName;
    numberOfCalls    = obj.numberOfCalls;
    totalTimeElapsed = obj.totalTimeElapsed;
}

/*!
	Assignment operator ProfileData.

	\param[in] obj ProfileData passed by reference.
	\return[out] ProfileData return by reference.
*/
ProfileData& ProfileData::operator=(const ProfileData &obj) {
   if (this == &obj){
       return *this;
   }

   timeLine          = obj.timeLine;
   functionName      = obj.functionName;
   numberOfCalls     = obj.numberOfCalls;
   totalTimeElapsed  = obj.totalTimeElapsed;
   return *this;
}

/*!
	Destructor ProfileData.

	\param[in] None.
*/
ProfileData::~ProfileData() {
}


/*!
	Dummy fucntion made for debug string. Not in use.

	\param[in] None.
	\param[Out] return debugging string.
*/
std::string ProfileData::debugString() {
    std::string retval;
    return retval;
}

/*!
	This function keep the record of current ProfileData and add the accumulated time also.

	\param[in] time_elasped time spent by current ProfileData .
*/
bool ProfileData::timeCall(long time_elasped) {
	timeLine.push_back(time_elasped);
   if(numberOfCalls++ > 50000){
      //return false;
   }
    totalTimeElapsed += time_elasped;
    return true;
	//profile("CLOCKS_PER_SEC: %ld", CLOCKS_PER_SEC);
}

/*!
	This function give the average time spent in the ProfileData.

	\return[out] average time of ProfileData metric.
*/
double ProfileData::averageTime() {
    if (numberOfCalls <= 0) return 0.0;
    return (double)totalTimeElapsed / (double)numberOfCalls;
}


/*!
	This function give the total time spent in the ProfileData.

	\return[out] total time of ProfileData metric.
*/
long ProfileData::totalTime() {
    return totalTimeElapsed;
}
}

