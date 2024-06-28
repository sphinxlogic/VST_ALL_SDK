/*================================================================================================*/
/*
 *	Copyright 2014-2015 by Avid Technology, Inc.
 *	All rights reserved.
 *
 *	CONFIDENTIAL: This document contains confidential information.  Do not 
 *	read or examine this document unless you are an Avid Technology employee
 *	or have signed a non-disclosure agreement with Avid Technology which protects
 *	the	confidentiality of this document.  DO NOT DISCLOSE ANY INFORMATION 
 *	CONTAINED IN THIS DOCUMENT TO ANY THIRD-PARTY WITHOUT THE PRIOR WRITTEN	CONSENT 
 *	OF Avid Technology, INC.
 */
/*================================================================================================*/ 
#include "DemoDelay_Describe.h"
#include "HostProcessor/HostProcessor_Describe.h"
#include "HostProcessor_Comp/HostProcessor_Comp_Describe.h"

#include "AAX_ICollection.h"

// ***************************************************************************
// ROUTINE:	GetEffectDescriptions
// ***************************************************************************
AAX_Result GetEffectDescriptions( AAX_ICollection * outCollection )
{
    AAX_Result				result = AAX_SUCCESS;
	
	// This plug-in includes two separate Effects
	//	Delay: This effect implements a standard delay algorithm using the Host Processor interface. The
	//		behavior of this Effect is identical to that of the standard DemoDelay AudioSuite plug-in.
	//	Delay Comp: This effect demonstrates the Host Processor interface's random-access functionality
	//		by "compensating" for its algorithm's delay and place input samples back into their original
	//		location on the timeline.
	result = (AAX_SUCCESS == result) ? HostProcessor_GetEffectDescriptions( outCollection ) : result;
	result = (AAX_SUCCESS == result) ? HostProcessor_Comp_GetEffectDescriptions( outCollection ) : result;
	
	outCollection->SetManufacturerName( "Avid" );
	outCollection->AddPackageName( "DemoDelay HostProcessor" );
	outCollection->SetPackageVersion( 1 );		
	
	return result;
}
