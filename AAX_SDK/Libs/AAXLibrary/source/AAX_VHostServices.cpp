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

#include "AAX_VHostServices.h"
#include "AAX_UIDs.h"
#include "acfbaseapi.h"
#include "ACFPtr.h"

// ******************************************************************************************
// METHOD:	AAX_VHostServices
// ******************************************************************************************
AAX_VHostServices::AAX_VHostServices( IACFUnknown * pUnkHost ) :
	mIACFHostServices( NULL ),
	mIACFHostServices2( NULL )
{
	if ( pUnkHost )
	{
		// Get the component factory service from the host so we can create the 
		// built-in plug-in definition.
		ACFPtr<IACFComponentFactory> pFactory;
		if ( pUnkHost->QueryInterface(IID_IACFComponentFactory, (void **)&pFactory) == ACF_OK )
		{
			// Create the object and get the base interface for it.
			pFactory->CreateComponent(AAXCompID_HostServices, 0, IID_IAAXHostServicesV1, (void **)&mIACFHostServices);
			pFactory->CreateComponent(AAXCompID_HostServices, 0, IID_IAAXHostServicesV2, (void **)&mIACFHostServices2);
		}
	}
}

// ******************************************************************************************
// METHOD:	~AAX_VHostServices
// ******************************************************************************************
AAX_VHostServices::~AAX_VHostServices( )
{
}

// ******************************************************************************************
// METHOD:	Assert
// ******************************************************************************************
AAX_Result AAX_VHostServices::Assert ( const char * inFile, int32_t inLine, const char * inCondition )
{
	if (mIACFHostServices )
		return mIACFHostServices->Assert( inFile, inLine, inCondition );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	Trace
// ******************************************************************************************
AAX_Result AAX_VHostServices::Trace ( int32_t inPriority, const char * inMessage )
{
	if ( mIACFHostServices )
		return mIACFHostServices->Trace( inPriority, inMessage );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	StackTrace
// ******************************************************************************************
AAX_Result AAX_VHostServices::StackTrace ( int32_t inTracePriority, int32_t inStackTracePriority, const char * inMessage )
{
	if ( mIACFHostServices2 )
		return mIACFHostServices2->StackTrace( inTracePriority, inStackTracePriority, inMessage );
	
	return AAX_ERROR_UNIMPLEMENTED;
}
