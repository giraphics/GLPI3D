#ifndef __GLM1C1_HPP__
#define __GLM1C1_HPP__

/* Wrapper file for including GLM headers with sourrounding #pragmas to disable specific compiler/QAC warnings */

#ifdef __ghs__
#pragma ghs nowarning 1518
#endif

#ifdef QACPP_ANALYSIS_ONLY 
/* 2015-03-27, Heiko Rippen, SQE: Hubert Gast,
   Reason for disabling following warnings: the GLM is Open Source code and must not be changed.
   Warnings are checked, in the used configuration no problems are detected. */ 
#pragma PRQA_MESSAGES_OFF 0435   // Msg(8:0435) Division by zero, the behaviour is undefined.
#pragma PRQA_MESSAGES_OFF 2108   // Msg(6:2108) The member access specifiers are not in order.
#pragma PRQA_MESSAGES_OFF 2180   // Msg(6:2180) This constructor could be used when implicit conversions are required.
#pragma PRQA_MESSAGES_OFF 2406   // Msg(6:2406) This union allows for manipulation of the bits in a floating point type object through its integral member.
#pragma PRQA_MESSAGES_OFF 2427   // Msg(6:2427) Direct use of fundamental type.
#pragma PRQA_MESSAGES_OFF 3017   // Msg(6:3017) This cast allows manipulation of the bits in an object of floating point type.
#pragma PRQA_MESSAGES_OFF 4054   // Msg(6:4054) This constructor does not have an initialisation list.
#endif

#include "glm.hpp"
#include "gtc/quaternion.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/matrix_access.hpp"
#include "gtc/type_ptr.hpp"
#include "gtx/quaternion_gtx.hpp"

#ifdef QACPP_ANALYSIS_ONLY 
/* reactivate QAC warnings */ 
#pragma PRQA_MESSAGES_ON 0435
#pragma PRQA_MESSAGES_ON 2108
#pragma PRQA_MESSAGES_ON 2180
#pragma PRQA_MESSAGES_ON 2406
#pragma PRQA_MESSAGES_ON 2427
#pragma PRQA_MESSAGES_ON 3017
#pragma PRQA_MESSAGES_ON 4054
#endif

#ifdef __ghs__
#pragma ghs endnowarning
#endif

#endif //__GLM1C1_HPP__

