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

#include "DemoGain_Describe.h"
#include "DemoGain_Defs.h"

//AAX components and modules
#include "DemoGain_Alg.h"
#include "DemoGain_Parameters.h"
#include "DemoGain_DirectData.h"

#include "AAX_ICollection.h"
#include "AAX_IComponentDescriptor.h"
#include "AAX_IEffectDescriptor.h"
#include "AAX_IPropertyMap.h"
#include "AAX_Errors.h"
#include "AAX_Assert.h"

AAX_CEffectID kEffectID_DemoGain	= "com.avid.aax.sdk.demogain.example";


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
	//
	// Register context fields as private data
	err = outDesc->AddPrivateData ( eAlgPortID_BGBuffer,
									sizeof(SDemoGain_BGBuffer), 
								    AAX_ePrivateDataOptions_DefaultOptions );                        AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc->AddPrivateData ( eAlgPortID_CurGain,
								    sizeof (float),
								    AAX_ePrivateDataOptions_DefaultOptions );                        AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc->AddPrivateData ( eAlgPortID_DelaySamples,
								    sizeof (SDemoGain_Delay),
								    AAX_ePrivateDataOptions_DefaultOptions );                        AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc->AddPrivateData ( eAlgPortID_InstanceIndex,
								    sizeof (uint32_t),
									AAX_ePrivateDataOptions_DefaultOptions );                        AAX_ASSERT (err == AAX_SUCCESS);
	
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
	properties->AddProperty ( AAX_eProperty_PlugInID_TI, cDemoGain_PlugInID_TI );
#ifndef AAX_TI_BINARY_IN_DEVELOPMENT // Define this macro when using a debug TI DLL to allocate only 1 instance per chip
	properties->AddProperty ( AAX_eProperty_TI_InstanceCycleCount, 222 );
	properties->AddProperty ( AAX_eProperty_TI_SharedCycleCount, 107 );
	properties->AddProperty ( AAX_eProperty_TI_MaxInstancesPerChip, 12 );
#endif

    // Register callbacks
    //
    // Native
	err = outDesc->AddProcessProc_Native<SDemoGain_Alg_Context> ( DemoGain_AlgorithmProcessFunction, properties, DemoGain_AlgorithmInitFunction, DemoGain_AlgorithmBackgroundFunction );
	AAX_ASSERT (err == AAX_SUCCESS);
	//
	// TI (TDM)
	properties->AddProperty ( AAX_eProperty_DSP_AudioBufferLength, AAX_eAudioBufferLengthDSP_Default ); // Overwrites native value
	err = outDesc->AddProcessProc_TI ("DemoGain_BG_MM_TI_Example.dll", "AlgEntry", properties, "AlgInit", "BGEntry" );
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
	outDescriptor->AddName ( "Demo Gain (Background)" );
	outDescriptor->AddName ( "Demo Gain BG" );
	outDescriptor->AddName ( "DemoGainBG" );
	outDescriptor->AddName ( "DmGainBG" );
	outDescriptor->AddName ( "DGBG" );
	outDescriptor->AddName ( "DB" );
	outDescriptor->AddCategory ( AAX_ePlugInCategory_Example );
	
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
	//
	// Direct Data module
	err = outDescriptor->AddProcPtr( (void *) DemoGain_DirectData::Create, kAAX_ProcPtrID_Create_EffectDirectData);
	AAX_ASSERT (err == AAX_SUCCESS);

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
	outCollection->AddPackageName( "DemoGain Plug-In" );
	outCollection->AddPackageName( "DemoGain" );
	outCollection->AddPackageName( "DmGi" );		
	outCollection->SetPackageVersion( 1 );		
	
	return result;
}
