/*================================================================================================*/
/*
 *	Copyright 2009-2015 by Avid Technology, Inc.
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

#include "DemoGain_Describe.h"
#include "DelayComp_Describe.h"
#include "Delay_Describe.h"

#include "AAX_ICollection.h"

// ***************************************************************************
// ROUTINE:	GetEffectDescriptions
// ***************************************************************************
AAX_Result GetEffectDescriptions( AAX_ICollection * outCollection )
{
    AAX_Result				result = AAX_SUCCESS;
	
	::DelayComp_GetEffectDescriptions( outCollection );
	::Delay_GetEffectDescriptions( outCollection );
	
	outCollection->SetManufacturerName( "Avid" );
	outCollection->AddPackageName( "Delay + Delay Comp" );
	outCollection->AddPackageName( "DlyDlyCmp" );
	outCollection->AddPackageName( "DDC" );
	outCollection->SetPackageVersion( 1 );	
    
	return result;
}
