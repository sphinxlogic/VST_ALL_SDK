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
 *	\file  AAX_VHostServices.h
 *
 *	\brief Version-managed concrete HostServices class
 *
 */ 
/*================================================================================================*/
#ifndef AAX_VHOSTSERVICES_H
#define AAX_VHOSTSERVICES_H

#include "AAX_IHostServices.h"
#include "AAX.h"
#include "acfunknown.h"
#include "ACFPtr.h"
#include "AAX_IACFHostServices.h"


class IACFUnknown;
class AAX_IACFHostServices;

/**
 *	\brief Version-managed concrete \ref AAX_IHostServices class
 *
 */
class AAX_VHostServices : public AAX_IHostServices
{
public:
	AAX_VHostServices( IACFUnknown * pUnkHost );
	~AAX_VHostServices( );

	virtual AAX_Result Assert ( const char * inFile, int32_t inLine, const char * inCondition );
	virtual AAX_Result Trace ( int32_t inPriority, const char * inMessage );
	virtual AAX_Result StackTrace ( int32_t inTracePriority, int32_t inStackTracePriority, const char * inMessage );

private:
	ACFPtr<AAX_IACFHostServices>	mIACFHostServices;
	ACFPtr<AAX_IACFHostServices_V2>	mIACFHostServices2;
};



#endif //AAX_IAUTOMATIONDELEGATE_H



