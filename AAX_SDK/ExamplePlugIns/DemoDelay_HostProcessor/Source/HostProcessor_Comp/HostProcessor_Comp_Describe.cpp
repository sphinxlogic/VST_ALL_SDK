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
 */
/*================================================================================================*/
#include "HostProcessor_Comp_Describe.h"
#include "DemoDelay_Describe.h"
#include "DemoDelay_HostProcessor_Comp.h"
#include "DemoDelay_Parameters.h"
#include "DemoDelay_Defs.h"

#include "AAX_ICollection.h"
#include "AAX_IEffectDescriptor.h"
#include "AAX_IPropertyMap.h"
#include "AAX_Assert.h"


// ***************************************************************************
// ROUTINE:	Delay_GetPlugInDescription
// ***************************************************************************
static AAX_Result DelayComp_GetPlugInDescription( AAX_IEffectDescriptor * outDescriptor )
{
    AAX_IPropertyMap *			properties = outDescriptor->NewPropertyMap();
    
	properties->AddProperty ( AAX_eProperty_ManufacturerID, cDemoDelay_ManufactureID );
	properties->AddProperty ( AAX_eProperty_ProductID, cDemoDelay_ProductID );
	properties->AddProperty ( AAX_eProperty_PlugInID_AudioSuite, cDemoDelay_PlugInID_Comp_AudioSuite );
	properties->AddProperty ( AAX_eProperty_NumberOfInputs, AAX_eMaxAudioSuiteTracks );
	properties->AddProperty ( AAX_eProperty_NumberOfOutputs, AAX_eMaxAudioSuiteTracks );
   	properties->AddProperty ( AAX_eProperty_UsesRandomAccess, true );
    properties->AddProperty ( AAX_eProperty_UsesClientGUI, true );
    
	// Effect identifiers
	//
	outDescriptor->AddName ( "Demo Delay (HostProcessor Delay Comp)" );
	outDescriptor->AddName ( "Demo Delay (HP Delay Comp)" );
	outDescriptor->AddName ( "Demo Delay (HP Cmp)" );
	outDescriptor->AddName ( "DmDly HP Cmp" );
	outDescriptor->AddName ( "DmDly Cmp" );
	outDescriptor->AddName ( "DC" );
	
	AAX_Result err = outDescriptor->AddCategory ( AAX_ePlugInCategory_Example );
    
    err = outDescriptor->AddProcPtr((void *) DemoDelay_Parameters::Create, kAAX_ProcPtrID_Create_EffectParameters);
    err = outDescriptor->AddProcPtr((void *) DemoDelay_HostProcessor_Comp::Create, kAAX_ProcPtrID_Create_HostProcessor);
    
    err = outDescriptor->SetProperties(properties);
    
    return err;
    
}

// ***************************************************************************
// ROUTINE:	Reverse_GetEffectDescriptions
// ***************************************************************************
AAX_Result HostProcessor_Comp_GetEffectDescriptions( AAX_ICollection * outCollection )
{
	AAX_Result				result = AAX_SUCCESS;
	const AAX_CEffectID		effectID_DemoDelay_Comp	= "com.avid.aax.sdk.demodelay.hostprocessor.comp";
	AAX_IEffectDescriptor *	plugInDescriptor = outCollection->NewDescriptor();
	if ( plugInDescriptor )
	{
		result = DelayComp_GetPlugInDescription( plugInDescriptor );
		if ( result == AAX_SUCCESS )
			result = outCollection->AddEffect( effectID_DemoDelay_Comp, plugInDescriptor );
		AAX_ASSERT (result == AAX_SUCCESS);
	}
	else result = AAX_ERROR_NULL_OBJECT;
    
    return result;
}
