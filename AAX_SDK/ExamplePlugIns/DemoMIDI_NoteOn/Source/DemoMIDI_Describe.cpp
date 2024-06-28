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

#include "DemoMIDI_Describe.h"
#include "DemoMIDI_Defs.h"

//AAX Components
#include "DemoMIDI_Alg.h"
#include "DemoMIDI_AlgProc.cpp" //Need due to function template
#include "DemoMIDI_Parameters.h"

#include "AAX_ICollection.h"
#include "AAX_IComponentDescriptor.h"
#include "AAX_IEffectDescriptor.h"
#include "AAX_IPropertyMap.h"
#include "AAX_Errors.h"
#include "AAX_Assert.h"

AAX_CEffectID kEffectID_DemoMIDI	= "com.avid.aax.sdk.demomidi";


// ***************************************************************************
// ROUTINE:	DescribeAlgorithmComponent
// Algorithm component description
// ***************************************************************************
static void DescribeAlgorithmComponent( AAX_IComponentDescriptor * outDesc )
{
	AAX_Result					err = AAX_SUCCESS;
	
	// Describe the fields in the algorithm's context structure
	//
	// Subscribe context fields to host-provided services or information
	err = outDesc->AddAudioIn(eAlgFieldID_AudioIn);											AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc->AddAudioOut(eAlgFieldID_AudioOut);										AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc->AddAudioBufferLength(eAlgFieldID_BufferSize);							AAX_ASSERT (err == AAX_SUCCESS);
    //
	// Register context fields as communications destinations (i.e. input)
	err = outDesc->AddDataInPort ( eAlgPortID_BypassIn,		sizeof (int32_t) );				AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc->AddDataInPort ( eAlgPortID_CoefsGainIn,	sizeof (float) );				AAX_ASSERT (err == AAX_SUCCESS);
	//
	// Register MIDI nodes
	err = outDesc->AddMIDINode(eAlgPortID_MIDINodeIn, AAX_eMIDINodeType_LocalInput, "DemoMIDI", 0xffff);					AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc->AddMIDINode(eAlgPortID_MIDINodeOut, AAX_eMIDINodeType_LocalOutput, "DemoMIDIOut", 0xffff);				AAX_ASSERT (err == AAX_SUCCESS);
	//
	// Register private data
	err = outDesc->AddPrivateData(eAlgPortID_IsOn, sizeof(SDemoMIDI_NoteBits));			AAX_ASSERT (err == AAX_SUCCESS);
	//
	// Register meters
	AAX_ASSERT( eMeterTap_Count == sizeof(cDemoMIDI_MeterID)/sizeof(AAX_CTypeID) );
	err = outDesc->AddMeters ( eAlgFieldID_Meters, cDemoMIDI_MeterID, eMeterTap_Count );	AAX_ASSERT (err == AAX_SUCCESS);
	
	// Register processing callbacks
	//
	// Create a property map 
	AAX_IPropertyMap* properties = outDesc->NewPropertyMap();
	AAX_ASSERT (properties);
	if ( !properties ) return;
	//
	// Generic properties
	err |= properties->AddProperty ( AAX_eProperty_ManufacturerID, cDemoMIDI_ManufactureID );
	err |= properties->AddProperty ( AAX_eProperty_ProductID, cDemoMIDI_ProductID );
	err |= properties->AddProperty ( AAX_eProperty_CanBypass, true );
	err |= properties->AddProperty ( AAX_eProperty_UsesClientGUI, true ); // Uses auto-GUI
	AAX_ASSERT (err == AAX_SUCCESS);
	//
	// Stem format -specific properties
	err |= properties->AddProperty ( AAX_eProperty_InputStemFormat, AAX_eStemFormat_Mono );
	err |= properties->AddProperty ( AAX_eProperty_OutputStemFormat, AAX_eStemFormat_Mono );
	err |= properties->AddProperty ( AAX_eProperty_PlugInID_Native, cDemoMIDI_PlugInID_Native );
	err |= properties->AddProperty ( AAX_eProperty_PlugInID_AudioSuite, cDemoMIDI_PlugInID_AudioSuite );
	AAX_ASSERT (err == AAX_SUCCESS);
	//
    // Register callbacks
    //
    // AAX Native
	err = outDesc->AddProcessProc_Native ( DemoMIDI_AlgorithmProcessFunction <1, 1>, properties );
	AAX_ASSERT (err == AAX_SUCCESS);
}

// ***************************************************************************
// ROUTINE:	DemoMIDI_GetPlugInDescription
// ***************************************************************************
static AAX_Result DemoMIDI_GetPlugInDescription( AAX_IEffectDescriptor * outDescriptor )
{
	int							err = AAX_SUCCESS;
	AAX_IComponentDescriptor *	compDesc = outDescriptor->NewComponentDescriptor ();
	if ( !compDesc )
		return AAX_ERROR_NULL_OBJECT;
	
	// Effect identifiers
	//
	outDescriptor->AddName ( "Demo MIDI NoteOn" );
	outDescriptor->AddName ( "DemoMIDI NoteOn" );
	outDescriptor->AddName ( "Demo MIDI" );
	outDescriptor->AddName ( "DmMIDI" );
	outDescriptor->AddName ( "DmMD" );
	outDescriptor->AddName ( "DM" );
	outDescriptor->AddCategory ( AAX_ePlugInCategory_Example );
	outDescriptor->AddCategory ( AAX_ePlugInCategory_SWGenerators );
	
	// Effect components
	//
	// Algorithm component
	DescribeAlgorithmComponent( compDesc );
	err = outDescriptor->AddComponent( compDesc );
	AAX_ASSERT (err == AAX_SUCCESS);
	//
	// Data model
	err = outDescriptor->AddProcPtr( (void *) DemoMIDI_Parameters::Create, kAAX_ProcPtrID_Create_EffectParameters );
	AAX_ASSERT (err == AAX_SUCCESS);
	outDescriptor->AddResourceInfo(AAX_eResourceType_PageTable, "DemoMIDIPages.xml");
	
	// Effect's meter display properties
	//
	AAX_IPropertyMap* meterProperties = outDescriptor->NewPropertyMap();
	if ( !meterProperties ) return AAX_ERROR_NULL_OBJECT;
    //
	meterProperties->AddProperty ( AAX_eProperty_Meter_Type, AAX_eMeterType_Input );
	meterProperties->AddProperty ( AAX_eProperty_Meter_Orientation, AAX_eMeterOrientation_Default );
	outDescriptor->AddMeterDescription( cDemoMIDI_MeterID[eMeterTap_PreGain], "Input", meterProperties );
    //
	meterProperties = outDescriptor->NewPropertyMap();
	if ( !meterProperties ) return AAX_ERROR_NULL_OBJECT;
    //
	meterProperties->AddProperty ( AAX_eProperty_Meter_Type, AAX_eMeterType_Output );
	meterProperties->AddProperty ( AAX_eProperty_Meter_Orientation, AAX_eMeterOrientation_Default );
	outDescriptor->AddMeterDescription( cDemoMIDI_MeterID[eMeterTap_PostGain], "Output", meterProperties );
	
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
		result = DemoMIDI_GetPlugInDescription( plugInDescriptor );
		if ( result == AAX_SUCCESS )
			result = outCollection->AddEffect( kEffectID_DemoMIDI, plugInDescriptor );
		AAX_ASSERT (result == AAX_SUCCESS);
	}
	else result = AAX_ERROR_NULL_OBJECT;
	
	outCollection->SetManufacturerName( "Avid" );
	outCollection->AddPackageName( "DemoMIDI Plug-In" );
	outCollection->AddPackageName( "DemoMIDI" );
	outCollection->AddPackageName( "DmMD" );		
	outCollection->SetPackageVersion( 1 );		
	
	return result;
}

