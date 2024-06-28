/*================================================================================================*/
/*
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
/*================================================================================================*/


#include "AAX.h"
#include "AAX_CHostServices.h"
#include "AAX_VHostServices.h"
#include <stdarg.h>
#include <stdio.h>

static AAX_VHostServices* sHostServices = NULL;

// ***************************************************************************
// METHOD:	Set
// ***************************************************************************
void AAX_CHostServices::Set ( IACFUnknown * pUnkHost )
{
	if ( !sHostServices && pUnkHost )
	{
		sHostServices = new AAX_VHostServices(pUnkHost);
	}
	else if ( sHostServices && !pUnkHost )
	{
		delete sHostServices;
		sHostServices = NULL;
	}
}

// ***************************************************************************
// METHOD:	Assert
// ***************************************************************************
AAX_Result AAX_CHostServices::Assert ( const char * inFile, int32_t inLine, const char * inCondition )
{
	//Unit tests and such don't support tracing.  Let's bail if tracing isn't setup.
	if (sHostServices == 0)
		return AAX_SUCCESS;

	return sHostServices->Assert ( inFile, inLine, inCondition );
}

// ***************************************************************************
// METHOD:	Trace
// ***************************************************************************
AAX_Result AAX_CHostServices::Trace ( AAX_ETracePriorityHost inPriority, const char * inFormat, ... )
{
	//Unit tests and such don't support tracing.  Let's bail if tracing isn't setup.
	if (sHostServices == 0)
		return AAX_SUCCESS;

	va_list	vargs;
	char	message [ 512 ];

	va_start ( vargs, inFormat );
	vsprintf( message, inFormat, vargs );
	va_end ( vargs );

	return sHostServices->Trace ( (int32_t)inPriority, message );
}

// ***************************************************************************
// METHOD:	StackTrace
// ***************************************************************************
AAX_Result AAX_CHostServices::StackTrace ( AAX_ETracePriorityHost inTracePriority, AAX_ETracePriorityHost inStackTracePriority, const char * inFormat, ... )
{
	//Unit tests and such don't support tracing.  Let's bail if tracing isn't setup.
	if (sHostServices == 0)
		return AAX_SUCCESS;
	
	va_list	vargs;
	char	message [ 512 ];
	
	va_start ( vargs, inFormat );
	vsprintf( message, inFormat, vargs );
	va_end ( vargs );
	
	return sHostServices->StackTrace ( (int32_t)inTracePriority, (int32_t)inStackTracePriority, message );
}
