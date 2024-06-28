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
 *	\file  AAX_IHostServices.h
 *
 *	\brief Various host services
 *
 */ 
/*================================================================================================*/


#ifndef AAX_IHOSTSERVICES_H
#define AAX_IHOSTSERVICES_H

#include "AAX.h"

/**	\brief Interface to diagnostic and debugging services provided by the %AAX host
	
	\details
	\hostimp
	
	\sa \ref AAX_IACFHostServices
*/
class AAX_IHostServices
{
public:
	
	virtual ~AAX_IHostServices() {}
	
	/**	\brief Assert that a condition is true
	 */
	virtual AAX_Result Assert ( const char * iFile, int32_t iLine, const char * iCondition ) = 0;
	/**	\brief Log a trace message
	 */
	virtual AAX_Result Trace ( int32_t iPriority, const char * iMessage ) = 0;
	/** \brief Log a trace message or a stack trace
	 */
	virtual AAX_Result StackTrace ( int32_t inTracePriority, int32_t inStackTracePriority, const char * inMessage ) = 0;
};

#endif // #ifndef AAX_IHOSTSERVICES_H
