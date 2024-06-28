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

#include "DemoGain_Describe.h"
#include "DemoGain_Defs.h"

//AAX Components
#include "DemoGain_Alg.h"
#include "DemoGain_Parameters.h"

#include "AAX_ICollection.h"
#include "AAX_IComponentDescriptor.h"
#include "AAX_IEffectDescriptor.h"
#include "AAX_IPropertyMap.h"
#include "AAX_Errors.h"
#include "AAX_Assert.h"

AAX_CEffectID kEffectID_DemoGain	= "com.avid.aax.sdk.demogain";


// ***************************************************************************
// ROUTINE:	DescribeAlgorithmComponent
// Algorithm component description
// ***************************************************************************
static void DescribeAlgorithmComponent( AAX_IComponentDescriptor * outDesc )
{
	AAX_Result					err = AAX_SUCCESS;
	
	// Describe algorithm's context structure
	//
	// Subscribe context fields to host-provided services or information
	err = outDesc->AddAudioIn (eAlgFieldID_AudioIn);                  AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc->AddAudioOut (eAlgFieldID_AudioOut);                AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc->AddAudioBufferLength (eAlgFieldID_BufferSize);     AAX_ASSERT (err == AAX_SUCCESS);
	AAX_ASSERT( eMeterTap_Count == sizeof(cDemoGain_MeterID)/sizeof(AAX_CTypeID) );
	err = outDesc->AddMeters ( eAlgFieldID_Meters, cDemoGain_MeterID, eMeterTap_Count );     AAX_ASSERT (err == AAX_SUCCESS);
    //
	// Register context fields as communications destinations (i.e. input)
	err = outDesc->AddDataInPort ( eAlgPortID_BypassIn, sizeof (int32_t) );                  AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc->AddDataInPort ( eAlgPortID_CoefsGainIn, sizeof (SDemoGain_CoefsGain) );   AAX_ASSERT (err == AAX_SUCCESS);
	
	// Register processing callbacks
	//
	// Create a property map 
	AAX_IPropertyMap* properties = outDesc->NewPropertyMap();
	AAX_ASSERT (properties);
	if ( !properties ) return;
	//
	// Generic properties
	properties->AddProperty ( AAX_eProperty_ManufacturerID, cDemoGain_ManufactureID );
	properties->AddProperty ( AAX_eProperty_ProductID, cDemoGain_ProductID );
	properties->AddProperty ( AAX_eProperty_CanBypass, true );
	properties->AddProperty ( AAX_eProperty_UsesClientGUI, true ); // Uses auto-GUI
	//
	// Stem format -specific properties
	properties->AddProperty ( AAX_eProperty_InputStemFormat, AAX_eStemFormat_Mono );
	properties->AddProperty ( AAX_eProperty_OutputStemFormat, AAX_eStemFormat_Mono );
	//
	properties->AddProperty ( AAX_eProperty_PlugInID_Native, cDemoGain_PlugInID_Native );
	properties->AddProperty ( AAX_eProperty_PlugInID_AudioSuite, cDemoGain_PlugInID_AudioSuite );	// Since this is a linear plug-in the Native version can also be an AudioSuite version.

    // Register Native callback
	err = outDesc->AddProcessProc_Native ( DemoGain_AlgorithmProcessFunction, properties );
	AAX_ASSERT (err == AAX_SUCCESS);

	// TI-specific properties
#ifndef AAX_TI_BINARY_IN_DEVELOPMENT // Define this macro when using a debug TI DLL to allocate only 1 instance per chip
	properties->AddProperty ( AAX_eProperty_TI_InstanceCycleCount, 102 );
	properties->AddProperty ( AAX_eProperty_TI_SharedCycleCount, 70 );
#endif
	properties->AddProperty ( AAX_eProperty_PlugInID_TI, cDemoGain_PlugInID_TI );
	properties->AddProperty ( AAX_eProperty_DSP_AudioBufferLength, AAX_eAudioBufferLengthDSP_Default ); 

	// Register TI callback
	err = outDesc->AddProcessProc_TI ("DemoGain_MM_TI_Example.dll", "AlgEntry", properties );
	AAX_ASSERT (err == AAX_SUCCESS);
}

// ***************************************************************************
// ROUTINE:	DemoGain_GetPlugInDescription
// ***************************************************************************
static AAX_Result DemoGain_GetPlugInDescription( AAX_IEffectDescriptor * outDescriptor )
{
	int							err = AAX_SUCCESS;
	AAX_IComponentDescriptor *	compDesc = outDescriptor->NewComponentDescriptor ();
	if ( !compDesc )
		return AAX_ERROR_NULL_OBJECT;

	// Effect identifiers
	//
	outDescriptor->AddName ( "Demo Gain" );
	outDescriptor->AddName ( "DemoGain" );
	outDescriptor->AddName ( "DmGain" );
	outDescriptor->AddName ( "DGpr" );
	outDescriptor->AddName ( "Dn" );
	outDescriptor->AddCategory ( AAX_ePlugInCategory_Example | AAX_ePlugInCategory_Dynamics );
	
	// Effect components
	//
	// Algorithm component
	DescribeAlgorithmComponent( compDesc );
	err = outDescriptor->AddComponent( compDesc );
	AAX_ASSERT (err == AAX_SUCCESS);
	//
	// Data model
	err = outDescriptor->AddProcPtr( (void *) DemoGain_Parameters::Create, kAAX_ProcPtrID_Create_EffectParameters );
	AAX_ASSERT (err == AAX_SUCCESS);
	outDescriptor->AddResourceInfo ( AAX_eResourceType_PageTable, "DemoGainPages.xml" );

	// Effect's meter display properties
	//
	AAX_IPropertyMap* meterProperties = outDescriptor->NewPropertyMap();
	if ( !meterProperties ) return AAX_ERROR_NULL_OBJECT;
    //
	meterProperties->AddProperty ( AAX_eProperty_Meter_Type, AAX_eMeterType_Input );
	meterProperties->AddProperty ( AAX_eProperty_Meter_Orientation, AAX_eMeterOrientation_Default );
	outDescriptor->AddMeterDescription( cDemoGain_MeterID[eMeterTap_PreGain], "Input", meterProperties );
    //
	meterProperties = outDescriptor->NewPropertyMap();
	if ( !meterProperties ) return AAX_ERROR_NULL_OBJECT;
    //
	meterProperties->AddProperty ( AAX_eProperty_Meter_Type, AAX_eMeterType_Output );
	meterProperties->AddProperty ( AAX_eProperty_Meter_Orientation, AAX_eMeterOrientation_Default );
	outDescriptor->AddMeterDescription( cDemoGain_MeterID[eMeterTap_PostGain], "Output", meterProperties );
		
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
		result = DemoGain_GetPlugInDescription( plugInDescriptor );
		if ( result == AAX_SUCCESS )
			result = outCollection->AddEffect( kEffectID_DemoGain, plugInDescriptor );
		AAX_ASSERT (result == AAX_SUCCESS);
	}
	else result = AAX_ERROR_NULL_OBJECT;
	
	outCollection->SetManufacturerName( "Avid" );
	outCollection->AddPackageName( "DemoGain AAX Plug-In" );
	outCollection->AddPackageName( "DemoGain Plug-In" );
	outCollection->AddPackageName( "DemoGain" );
	outCollection->AddPackageName( "DmGi" );		
	outCollection->SetPackageVersion( 1 );		
	
	return result;
}
