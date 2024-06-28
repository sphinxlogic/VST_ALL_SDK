/*================================================================================================*/
/*
 *
 *	Copyright 2014-2015 by Avid Technology, Inc.
 *	All rights reserved.
 *	
 *	CONFIDENTIAL: This document contains confidential information.  Do not 
 *	read or examine this document unless you are an Avid Technology employee
 *	or have signed a non-disclosure agreement with Avid Technology which protects
 *	the	confidentiality of this document.  DO NOT DISCLOSE ANY INFORMATION 
 *	CONTAINED IN THIS DOCUMENT TO ANY THIRD-PARTY WITHOUT THE PRIOR WRITTEN	CONSENT 
 *	OF Avid Technology, INC.
 *
 */

/**  
 *	\file  AAX_CHostServices.h
 *
 *	\brief Concrete implementation of the AAX_IHostServices interface.
 *
 */ 
/*================================================================================================*/


#ifndef AAX_CHOSTSERVICES_H
#define AAX_CHOSTSERVICES_H

#include "AAX.h"
#include "AAX_Enums.h"


class IACFUnknown;

/**	@brief Concrete implementation of the \ref AAX_IHostServices interface
 */
class AAX_CHostServices
{
public:
	static void Set ( IACFUnknown * pUnkHost );
	
	static AAX_Result Assert ( const char * inFile, int32_t inLine, const char * inCondition );
	static AAX_Result Trace ( AAX_ETracePriorityHost inPriority, const char * inFormat, ... );
	static AAX_Result StackTrace ( AAX_ETracePriorityHost inTracePriority, AAX_ETracePriorityHost inStackTracePriority, const char * inFormat, ... );
};


#endif
