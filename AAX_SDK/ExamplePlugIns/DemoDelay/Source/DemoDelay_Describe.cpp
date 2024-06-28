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
#include "DemoDelay_Defs.h"

//AAX Components
#include "DemoDelay_Alg.h"
#include "DemoDelay_Parameters.h"

#include "CSimpleDelayLine.h"

#include "AAX_ICollection.h"
#include "AAX_IComponentDescriptor.h"
#include "AAX_IEffectDescriptor.h"
#include "AAX_IPropertyMap.h"
#include "AAX_Errors.h"
#include "AAX_Assert.h"

AAX_CEffectID kEffectID_DemoDelay	= "com.avid.aax.sdk.demodelay";


// ***************************************************************************
// ROUTINE:	DescribeAlgorithmComponent
// Algorithm component description
// ***************************************************************************
static void DescribeAlgorithmComponent( AAX_IComponentDescriptor * outDesc )
{
	AAX_Result					err = AAX_SUCCESS;
		
	// Subscribe context fields to host-provided services or information
	err |= outDesc->AddAudioIn (AAX_FIELD_INDEX (SDemoDelay_AlgContext, mInputPP));
	err |= outDesc->AddAudioOut (AAX_FIELD_INDEX (SDemoDelay_AlgContext, mOutputPP));
	err |= outDesc->AddAudioBufferLength (AAX_FIELD_INDEX (SDemoDelay_AlgContext, mBufferLengthP));
	AAX_ASSERT( eMeterTap_Count == sizeof(cDemoDelay_MeterID)/sizeof(AAX_CTypeID) );
	err |= outDesc->AddMeters ( AAX_FIELD_INDEX (SDemoDelay_AlgContext, mMeterOutputPP), cDemoDelay_MeterID, eMeterTap_Count );
    
	// Register context fields as communications destinations (i.e. input)
	err |= outDesc->AddDataInPort ( AAX_FIELD_INDEX (SDemoDelay_AlgContext, mCoefsDelayP), sizeof (SDemoDelay_Coefs) );
	err |= outDesc->AddDataInPort ( AAX_FIELD_INDEX (SDemoDelay_AlgContext, mDelaySamplesP), sizeof (int32_t) );
    AAX_ASSERT (err == AAX_SUCCESS);
	
	// Register private data fields
	err |= outDesc->AddPrivateData ( AAX_FIELD_INDEX(SDemoDelay_AlgContext, mDelayLineP), sizeof(CSimpleDelayLine), AAX_ePrivateDataOptions_External);
	AAX_ASSERT (err == AAX_SUCCESS);
		
	// Create a property map 
	AAX_IPropertyMap* properties = outDesc->NewPropertyMap();
	AAX_ASSERT (properties);
	if ( !properties ) return;
	
	// Generic properties
	properties->AddProperty ( AAX_eProperty_ManufacturerID, cDemoDelay_ManufactureID );
	properties->AddProperty ( AAX_eProperty_ProductID, cDemoDelay_ProductID );
	properties->AddProperty ( AAX_eProperty_CanBypass, true );
	properties->AddProperty ( AAX_eProperty_UsesClientGUI, true ); // Uses auto-GUI
	
	// Stem format -specific properties
	properties->AddProperty ( AAX_eProperty_InputStemFormat, AAX_eStemFormat_Mono );
	properties->AddProperty ( AAX_eProperty_OutputStemFormat, AAX_eStemFormat_Mono );
    
	properties->AddProperty ( AAX_eProperty_PlugInID_AudioSuite, cDemoDelay_PlugInID_AudioSuite );
	properties->AddProperty ( AAX_eProperty_PlugInID_Native, cDemoDelay_PlugInID_Native );
	properties->AddProperty ( AAX_eProperty_PlugInID_TI, cDemoDelay_PlugInID_TI );

#ifndef AAX_TI_BINARY_IN_DEVELOPMENT // Define this macro when using a debug TI DLL to allocate only 1 instance per chip
	properties->AddProperty ( AAX_eProperty_TI_InstanceCycleCount, 1420 );
	properties->AddProperty ( AAX_eProperty_TI_SharedCycleCount, 160 );
#endif
    
    // Register Native Callback
	err = outDesc->AddProcessProc_Native ( DemoDelay_AlgorithmProcessFunction, properties );
	AAX_ASSERT (err == AAX_SUCCESS);
	
	// Register TI Callback
	properties->AddProperty ( AAX_eProperty_DSP_AudioBufferLength, AAX_eAudioBufferLengthDSP_Default ); 
	err = outDesc->AddProcessProc_TI ("DemoDelayAlgMono.dll", "AlgEntry", properties );
	AAX_ASSERT (err == AAX_SUCCESS);
}

// ***************************************************************************
// ROUTINE:	DemoDelay_GetPlugInDescription
// ***************************************************************************
static AAX_Result DemoDelay_GetPlugInDescription( AAX_IEffectDescriptor * outDescriptor )
{
	int							err = AAX_SUCCESS;
	AAX_IComponentDescriptor *	compDesc = outDescriptor->NewComponentDescriptor ();
	if ( !compDesc )
		return AAX_ERROR_NULL_OBJECT;

	// Effect identifiers
	//
	outDescriptor->AddName ( "Demo Delay" );
	outDescriptor->AddName ( "DmDly" );

	outDescriptor->AddCategory ( AAX_ePlugInCategory_Example );
	
	// Effect components
	//
	// Algorithm component
	DescribeAlgorithmComponent( compDesc );
	err = outDescriptor->AddComponent( compDesc );
	AAX_ASSERT (err == AAX_SUCCESS);
	//
	// Data model
	err = outDescriptor->AddProcPtr( (void *) DemoDelay_Parameters::Create, kAAX_ProcPtrID_Create_EffectParameters );
	AAX_ASSERT (err == AAX_SUCCESS);
	outDescriptor->AddResourceInfo ( AAX_eResourceType_PageTable, "DemoDelayPages.xml" );

	// Effect's meter display properties
	//
	AAX_IPropertyMap* meterProperties = outDescriptor->NewPropertyMap();
	if ( !meterProperties ) return AAX_ERROR_NULL_OBJECT;
    //
	meterProperties->AddProperty ( AAX_eProperty_Meter_Type, AAX_eMeterType_Input );
	meterProperties->AddProperty ( AAX_eProperty_Meter_Orientation, AAX_eMeterOrientation_Default );
	outDescriptor->AddMeterDescription( cDemoDelay_MeterID[eMeterTap_Input], "Input", meterProperties );
    //
	meterProperties = outDescriptor->NewPropertyMap();
	if ( !meterProperties ) return AAX_ERROR_NULL_OBJECT;
    //
	meterProperties->AddProperty ( AAX_eProperty_Meter_Type, AAX_eMeterType_Output );
	meterProperties->AddProperty ( AAX_eProperty_Meter_Orientation, AAX_eMeterOrientation_Default );
	outDescriptor->AddMeterDescription( cDemoDelay_MeterID[eMeterTap_Output], "Output", meterProperties );
		
	return AAX_SUCCESS;
}

// ***************************************************************************
// ROUTINE:	GetEffectDescriptions
// ***************************************************************************
AAX_Result GetEffectDescriptions( AAX_ICollection * outCollection )
{
	AAX_Result				result = AAX_SUCCESS;
	AAX_IEffectDescriptor *	plugInDescriptor = outCollection->NewDescriptor();
	if ( plugInDescriptor )
	{
		result = DemoDelay_GetPlugInDescription( plugInDescriptor );
		if ( result == AAX_SUCCESS )
			result = outCollection->AddEffect( kEffectID_DemoDelay, plugInDescriptor );
		AAX_ASSERT (result == AAX_SUCCESS);
	}
	else result = AAX_ERROR_NULL_OBJECT;
	
	outCollection->SetManufacturerName( "Avid" );
	outCollection->AddPackageName( "DemoDelay" );
	outCollection->SetPackageVersion( 1 );		
	
	return result;
}
