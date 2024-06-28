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

#include "DemoDist_Describe.h"

#include "DemoDistDefs.h"

// AAX Components
#include "DemoDist_Alg.h"
#include "DemoDist_AlgProc.cpp" // Needed due to function template
#include "DemoDist_Parameters.h"

#include "AAX_ICollection.h"
#include "AAX_IComponentDescriptor.h"
#include "AAX_IEffectDescriptor.h"

#include "AAX_IPropertyMap.h"
#include "AAX_Errors.h"
#include "AAX_Assert.h"


AAX_CEffectID kEffectID_DemoDist = "com.avid.aax.sdk.demodist.gc";





// ***************************************************************************
// ROUTINE:	DescribeAlgorithmComponent
// Algorithm component description
// ***************************************************************************
void DescribeAlgComponent ( AAX_EStemFormat inStemFormat, AAX_IComponentDescriptor &	outDesc)
{
	AAX_Result					err = AAX_SUCCESS;
	AAX_ASSERT (inStemFormat == AAX_eStemFormat_Mono || inStemFormat == AAX_eStemFormat_Stereo);
	
	// Describe algorithm's context structure
	//
	// Subscribe context fields to host-provided services or information
	err = outDesc.AddAudioIn (eDemoDist_AlgFieldID_AudioIn);					AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc.AddAudioOut (eDemoDist_AlgFieldID_AudioOut);					AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc.AddAudioBufferLength (eDemoDist_AlgFieldID_BufferLength);		AAX_ASSERT (err == AAX_SUCCESS);
	//
	// Register meter fields
	switch( inStemFormat )
	{
		case AAX_eStemFormat_Mono:
		{
			AAX_ASSERT( SMeterTapsM::eMeterTap_Count == sizeof(cDemoDist_MeterID_Mono)/sizeof(AAX_CTypeID) );
			err = outDesc.AddMeters ( eAlgFieldID_MeterTaps, cDemoDist_MeterID_Mono, SMeterTapsM::eMeterTap_Count ); AAX_ASSERT (err == AAX_SUCCESS);
		} break;
		case AAX_eStemFormat_Stereo:
		{
			AAX_ASSERT( SMeterTapsS::eMeterTap_Count == sizeof(cDemoDist_MeterID_Stereo)/sizeof(AAX_CTypeID) );
			err = outDesc.AddMeters ( eAlgFieldID_MeterTaps, cDemoDist_MeterID_Stereo, SMeterTapsS::eMeterTap_Count ); AAX_ASSERT (err == AAX_SUCCESS);			
		} break;
		default:
			break;
	}
	//
	// Register context fields as communications destinations (i.e. input)
	err = outDesc.AddDataInPort (eAlgPortID_BypassIn, sizeof (int32_t));					AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc.AddDataInPort (eAlgPortID_MixIn, sizeof (float));							AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc.AddDataInPort (eAlgPortID_InpGIn, sizeof (float));						AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc.AddDataInPort (eAlgPortID_OutGIn, sizeof (float));						AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc.AddDataInPort (eAlgPortID_CoefsDistIn, sizeof (SDemoDist_DistCoefs));		AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc.AddDataInPort (eAlgPortID_CoefsFilterIn, sizeof (SDemoDist_FiltCoefs));	AAX_ASSERT (err == AAX_SUCCESS);
	//
	// Register private data fields
	int32_t dataBlockSize = sizeof (CSimpleBiquad);
	if (inStemFormat == AAX_eStemFormat_Stereo)
		dataBlockSize *= 2;
	err = outDesc.AddPrivateData (eAlgFieldID_Biquads, dataBlockSize,
                                  AAX_ePrivateDataOptions_DefaultOptions);							AAX_ASSERT (err == AAX_SUCCESS);
	
	
	// Register processing callbacks
	//
	// Create a property map
	AAX_IPropertyMap* properties = outDesc.NewPropertyMap();
	AAX_ASSERT (properties);
	if ( !properties ) return;
	//
	// Generic properties	
	properties->AddProperty ( AAX_eProperty_ManufacturerID, cDemoDist_ManufactureID );
	properties->AddProperty ( AAX_eProperty_ProductID, cDemoDist_ProductID );
	properties->AddProperty ( AAX_eProperty_InputStemFormat, inStemFormat );
	properties->AddProperty ( AAX_eProperty_OutputStemFormat, inStemFormat );
	properties->AddProperty ( AAX_eProperty_CanBypass, true );
	properties->AddProperty ( AAX_eProperty_UsesClientGUI, true ); 	// Register for auto-GUI
	//
	// Stem format -specific properties
	switch( inStemFormat )
	{
		case AAX_eStemFormat_Mono:
		{
			// IDs
			properties->AddProperty ( AAX_eProperty_PlugInID_Native, cDemoDist_TypeID_MonoNative );
			properties->AddProperty ( AAX_eProperty_PlugInID_AudioSuite, cDemoDist_TypeID_AS );
			properties->AddProperty ( AAX_eProperty_PlugInID_TI, cDemoDist_TypeID_MonoTI );
				
			// Resource requirements
#ifndef AAX_TI_BINARY_IN_DEVELOPMENT // Define this macro when using a debug TI DLL to allocate only 1 instance per chip
			properties->AddProperty ( AAX_eProperty_TI_InstanceCycleCount, 640 );
			properties->AddProperty ( AAX_eProperty_TI_SharedCycleCount, 220 );
#endif // _DEBUG
			
			// Register callbacks
			//
			// Native (Native and AudioSuite)
			err = outDesc.AddProcessProc_Native (DemoDist_AlgorithmProcessFunction <1, 1>, properties);
			AAX_ASSERT (err == AAX_SUCCESS);
			//
			// TI (TDM)
			properties->AddProperty (AAX_eProperty_DSP_AudioBufferLength, AAX_eAudioBufferLengthDSP_Default);
			err = outDesc.AddProcessProc_TI ("DemoDist_GenCoef_MM_TI_Example.dll", "AlgEntry", properties);
			AAX_ASSERT (err == AAX_SUCCESS);
		} break;
			
		case AAX_eStemFormat_Stereo:
		{
			// IDs
			properties->AddProperty ( AAX_eProperty_PlugInID_Native, cDemoDist_TypeID_StereoNative );
			properties->AddProperty ( AAX_eProperty_PlugInID_TI, cDemoDist_TypeID_StereoTI );
			
			// Resource requirements
#ifndef AAX_TI_BINARY_IN_DEVELOPMENT // Define this macro when using a debug TI DLL to allocate only 1 instance per chip
			properties->AddProperty ( AAX_eProperty_TI_InstanceCycleCount, 1250 );
			properties->AddProperty ( AAX_eProperty_TI_SharedCycleCount, 242 );
#endif // _DEBUG
			
			// Register callbacks
			//
			// Native (Native and AudioSuite)
			err = outDesc.AddProcessProc_Native (DemoDist_AlgorithmProcessFunction<2, 2>, properties);
			AAX_ASSERT (err == AAX_SUCCESS);
			//
			// TI (TDM)
			properties->AddProperty (AAX_eProperty_DSP_AudioBufferLength, AAX_eAudioBufferLengthDSP_Default);
			err = outDesc.AddProcessProc_TI ("DemoDist_GenCoef_SS_TI_Example.dll", "AlgEntry", properties);
			AAX_ASSERT (err == AAX_SUCCESS);
		} break;
			
		default:
			break;
	}		
}

// ***************************************************************************
// ROUTINE:	DemoDist_GetPlugInDescription
// ***************************************************************************
AAX_Result DemoDist_GetPlugInDescription(AAX_IEffectDescriptor&	outDescriptor)
{
	int							err = AAX_SUCCESS;
	AAX_IComponentDescriptor *	compDesc = outDescriptor.NewComponentDescriptor ();	
	if ( !compDesc )
		return AAX_ERROR_NULL_OBJECT;
	
	// Effect identifiers
	//
	outDescriptor.AddName ( "Demo Dist (GenCoef)" );
	outDescriptor.AddName ( "Demo Dist" );
	outDescriptor.AddName ( "DmD" );
	outDescriptor.AddCategory ( AAX_ePlugInCategory_Example );
	
	// Effect components
	//
	// Mono algorithm component
	compDesc->Clear ();
	DescribeAlgComponent ( AAX_eStemFormat_Mono, *compDesc );
	err = outDescriptor.AddComponent ( compDesc );
	AAX_ASSERT (err == AAX_SUCCESS);
	//
	// Stereo algorithm component
	compDesc->Clear ();
	DescribeAlgComponent ( AAX_eStemFormat_Stereo, *compDesc );
	err = outDescriptor.AddComponent ( compDesc );
	AAX_ASSERT (err == AAX_SUCCESS);
	//
	// Data model
	err = outDescriptor.AddProcPtr( (void *) DemoDist_Parameters::Create, kAAX_ProcPtrID_Create_EffectParameters );
	AAX_ASSERT (err == AAX_SUCCESS);
	outDescriptor.AddResourceInfo ( AAX_eResourceType_PageTable, "DemoDist.xml" );
	
	// Effect's meter display properties
	//
	// Input (mono)
	AAX_IPropertyMap* meterProperties = outDescriptor.NewPropertyMap();
	if ( !meterProperties ) return AAX_ERROR_NULL_OBJECT;
	//
	meterProperties->AddProperty ( AAX_eProperty_Meter_Type, AAX_eMeterType_Input );
	meterProperties->AddProperty ( AAX_eProperty_Meter_Orientation, AAX_eMeterOrientation_Default );
	outDescriptor.AddMeterDescription( cDemoDist_MeterID_In, "Input", meterProperties );
	//
	// Input (left)
	meterProperties = outDescriptor.NewPropertyMap();
	if ( !meterProperties ) return AAX_ERROR_NULL_OBJECT;
	//
	meterProperties->AddProperty ( AAX_eProperty_Meter_Type, AAX_eMeterType_Input );
	meterProperties->AddProperty ( AAX_eProperty_Meter_Orientation, AAX_eMeterOrientation_Default );
	outDescriptor.AddMeterDescription( cDemoDist_MeterID_InL, "LeftIn", meterProperties );
	//
	// Input (right)
	meterProperties = outDescriptor.NewPropertyMap();
	if ( !meterProperties ) return AAX_ERROR_NULL_OBJECT;
	//
	meterProperties->AddProperty ( AAX_eProperty_Meter_Type, AAX_eMeterType_Input );
	meterProperties->AddProperty ( AAX_eProperty_Meter_Orientation, AAX_eMeterOrientation_Default );
	outDescriptor.AddMeterDescription( cDemoDist_MeterID_InR, "RightIn", meterProperties );
	//
	// Output (mono)
	meterProperties = outDescriptor.NewPropertyMap();
	if ( !meterProperties ) return AAX_ERROR_NULL_OBJECT;
	//
	meterProperties->AddProperty ( AAX_eProperty_Meter_Type, AAX_eMeterType_Output );
	meterProperties->AddProperty ( AAX_eProperty_Meter_Orientation, AAX_eMeterOrientation_Default );
	outDescriptor.AddMeterDescription( cDemoDist_MeterID_DistOut, "Output", meterProperties );
	//
	// Output (left)
	meterProperties = outDescriptor.NewPropertyMap();
	if ( !meterProperties ) return AAX_ERROR_NULL_OBJECT;
	//
	meterProperties->AddProperty ( AAX_eProperty_Meter_Type, AAX_eMeterType_Output );
	meterProperties->AddProperty ( AAX_eProperty_Meter_Orientation, AAX_eMeterOrientation_Default );
	outDescriptor.AddMeterDescription( cDemoDist_MeterID_DistL, "LeftOut", meterProperties );
	//
	// Output (right)
	meterProperties = outDescriptor.NewPropertyMap();
	if ( !meterProperties ) return AAX_ERROR_NULL_OBJECT;
	//
	meterProperties->AddProperty ( AAX_eProperty_Meter_Type, AAX_eMeterType_Output );
	meterProperties->AddProperty ( AAX_eProperty_Meter_Orientation, AAX_eMeterOrientation_Default );
	outDescriptor.AddMeterDescription( cDemoDist_MeterID_DistR, "RightOut", meterProperties );

	return AAX_SUCCESS;
}


// ***************************************************************************
// ROUTINE:	GetEffectDescriptions
// ***************************************************************************
AAX_Result GetEffectDescriptions ( AAX_ICollection * outCollection )
{
	AAX_Result result = AAX_SUCCESS;
	
	AAX_IEffectDescriptor* plugInDescriptor = outCollection->NewDescriptor();
	if ( plugInDescriptor )
	{
		result = DemoDist_GetPlugInDescription(*plugInDescriptor);
		if ( result == AAX_SUCCESS )
			outCollection->AddEffect ( kEffectID_DemoDist, plugInDescriptor );
	}
	else
	{
		result = AAX_ERROR_NULL_OBJECT;
	}
	
	outCollection->SetManufacturerName( "Avid" );
	outCollection->AddPackageName( "DemoDist Plug-In" );
	outCollection->AddPackageName( "DemoDistPlugIn" );
	outCollection->AddPackageName( "DemoDist" );		
	outCollection->AddPackageName( "DmDi" );		
	outCollection->SetPackageVersion( 1 );		
	
	return result;
}
