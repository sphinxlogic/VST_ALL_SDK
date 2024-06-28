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

#include "DelayComp_Describe.h"
#include "DelayComp_HostProcessor.h"

#include "AAX_ICollection.h"
#include "AAX_IEffectDescriptor.h"
#include "AAX_IPropertyMap.h"
#include "AAX_Assert.h"


AAX_CEffectID kEffectID_DelayComp	= "com.avid.aax.sdk.delaycomp";

// *******************************************************************************
// ROUTINE:	Create_DemoGainDelay_HostProcessor
// *******************************************************************************
static AAX_IHostProcessor * AAX_CALLBACK Create_DelayComp_HostProcessor()
{
    DelayComp_HostProcesser * hostProcessor = new DelayComp_HostProcesser();
    return hostProcessor;
}

// ***************************************************************************
// ROUTINE:	Delay_GetPlugInDescription
// ***************************************************************************
static AAX_Result DelayComp_GetPlugInDescription( AAX_IEffectDescriptor * outDescriptor )
{
    
    AAX_IPropertyMap *			properties = outDescriptor->NewPropertyMap();
    
	properties->AddProperty ( AAX_eProperty_ManufacturerID, cDemoGainDelay_ManufactureID );
	properties->AddProperty ( AAX_eProperty_ProductID, cDemoGainDelay_ProductID );
	properties->AddProperty ( AAX_eProperty_PlugInID_AudioSuite, cDemoGainDelayComp_PlugInID_AudioSuite );
	properties->AddProperty ( AAX_eProperty_NumberOfInputs, AAX_eMaxAudioSuiteTracks );
	properties->AddProperty ( AAX_eProperty_NumberOfOutputs, AAX_eMaxAudioSuiteTracks );
   	properties->AddProperty ( AAX_eProperty_UsesRandomAccess, true );
    properties->AddProperty ( AAX_eProperty_DisableHandles, true );
    properties->AddProperty ( AAX_eProperty_UsesClientGUI, true );
    
	// Effect identifiers
	//
	outDescriptor->AddName ( "Demo Gain Delay Comp" );
	outDescriptor->AddName ( "DemoGainDelayComp" );
	outDescriptor->AddName ( "DmGnDlCp" );
	outDescriptor->AddName ( "DGDlCp" );
   	outDescriptor->AddName ( "DGlCp" );
	outDescriptor->AddName ( "DC" );
	
	AAX_Result err = outDescriptor->AddCategory ( AAX_ePlugInCategory_Example );
	
    err = outDescriptor->AddProcPtr((void *) Create_DelayComp_HostProcessor, kAAX_ProcPtrID_Create_HostProcessor);
    
    err = outDescriptor->SetProperties(properties);
    
    return err;
    
}

// ***************************************************************************
// ROUTINE:	Reverse_GetEffectDescriptions
// ***************************************************************************
AAX_Result DelayComp_GetEffectDescriptions( AAX_ICollection * outCollection )
{
	AAX_Result				result = AAX_SUCCESS;
	AAX_IEffectDescriptor *	plugInDescriptor = outCollection->NewDescriptor();
	if ( plugInDescriptor )
	{
		result = DelayComp_GetPlugInDescription( plugInDescriptor );
		if ( result == AAX_SUCCESS )
			result = outCollection->AddEffect( kEffectID_DelayComp, plugInDescriptor );
		AAX_ASSERT (result == AAX_SUCCESS);
	}
	else result = AAX_ERROR_NULL_OBJECT;
    
    return result;
}

