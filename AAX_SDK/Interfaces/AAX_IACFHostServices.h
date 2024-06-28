/*================================================================================================*/
/*
 *
 *	Copyright 2013-2015 by Avid Technology, Inc.
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
 *	\file  AAX_IACFHostServices.h
 *
 */ 
/*================================================================================================*/


#ifndef AAX_IACFHOSTSERVICES_H
#define AAX_IACFHOSTSERVICES_H

#include "AAX.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

#include "acfunknown.h"

/**	\brief Versioned interface to diagnostic and debugging services provided by the %AAX host
 */
class AAX_IACFHostServices : public IACFUnknown
{
public:
	virtual AAX_Result Assert ( const char * iFile, int32_t iLine, const char * iCondition ) = 0;	///< \copydoc AAX_IHostServices::Assert()
	virtual AAX_Result Trace ( int32_t iPriority, const char * iMessage ) = 0;	///< \copydoc AAX_IHostServices::Trace()
};

/**	\brief V2 of versioned interface to diagnostic and debugging services provided by the %AAX host
 */
class AAX_IACFHostServices_V2 : public AAX_IACFHostServices
{
public:
	virtual AAX_Result StackTrace ( int32_t inTracePriority, int32_t inStackTracePriority, const char * inMessage ) = 0;	///< \copydoc AAX_IHostServices::StackTrace()
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif // #ifndef AAX_IACFHOSTSERVICES_H
