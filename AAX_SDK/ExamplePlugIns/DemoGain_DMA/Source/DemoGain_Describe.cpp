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

#include "DemoGain_AlgProc_ScatterGather.h"
#include "DemoGain_AlgProc_Burst.h"
#include "DemoGain_AlgCommon.h"
#include "DemoGain_Parameters.h"
#include "DemoGain_Defs.h"

#include "AAX_ICollection.h"
#include "AAX_IComponentDescriptor.h"
#include "AAX_IEffectDescriptor.h"
#include "AAX_IPropertyMap.h"
#include "AAX_Errors.h"
#include "AAX_Assert.h"

// Forward declarations
static void DemoGain_DMA_AddCommonAlgProperties (AAX_IPropertyMap* outProperties);
static void DescribeAlgorithmComponent_Common ( AAX_IComponentDescriptor * outDesc );
static void DescribeAlgorithmComponent_ScatterGather ( AAX_IComponentDescriptor * outDesc );
static void DescribeAlgorithmComponent_Burst ( AAX_IComponentDescriptor * outDesc );
static AAX_Result DemoGain_DMA_GetEffectDescription ( AAX_IEffectDescriptor * outDescriptor );
static AAX_Result DemoGain_DMASG_GetEffectDescription ( AAX_IEffectDescriptor * outDescriptor );
static AAX_Result DemoGain_DMABurst_GetEffectDescription ( AAX_IEffectDescriptor * outDescriptor );
AAX_Result GetEffectDescriptions ( AAX_ICollection * outCollection );



// ***************************************************************************
// ROUTINE:	DescribeAlgorithmComponent_Common
// Common DemoGain_DMA algorithm component properties description
// ***************************************************************************
static void DemoGain_DMA_AddCommonAlgProperties (AAX_IPropertyMap* outProperties)
{
	AAX_Result err = AAX_SUCCESS;
	
	// Generic properties
	//
	// IDs
	outProperties->AddProperty ( AAX_eProperty_ManufacturerID, cDemoGain_ManufactureID );                AAX_ASSERT (err == AAX_SUCCESS);
	outProperties->AddProperty ( AAX_eProperty_ProductID, cDemoGain_ProductID );                         AAX_ASSERT (err == AAX_SUCCESS);
	//
	// Feature support
	outProperties->AddProperty ( AAX_eProperty_CanBypass, true );                                        AAX_ASSERT (err == AAX_SUCCESS);
	outProperties->AddProperty ( AAX_eProperty_UsesClientGUI, true );                                    AAX_ASSERT (err == AAX_SUCCESS);
	outProperties->AddProperty ( AAX_eProperty_LatencyContribution, cDemoGain_DMA_LatencyContribution ); AAX_ASSERT (err == AAX_SUCCESS);
	//
	// Stem format
	outProperties->AddProperty ( AAX_eProperty_InputStemFormat, AAX_eStemFormat_Mono );                  AAX_ASSERT (err == AAX_SUCCESS);
	outProperties->AddProperty ( AAX_eProperty_OutputStemFormat, AAX_eStemFormat_Mono );                 AAX_ASSERT (err == AAX_SUCCESS);
	
}

// ***************************************************************************
// ROUTINE:	DescribeAlgorithmComponent_Common
// Common DemoGain_DMA algorithm component description
// ***************************************************************************
static void DescribeAlgorithmComponent_Common ( AAX_IComponentDescriptor * outDesc )
{
	AAX_Result err = AAX_SUCCESS;
	
	// Describe algorithm's context structure
	//
	// Subscribe context fields to host-provided services or information	
	err = outDesc->AddAudioIn (eAlgFieldID_AudioIn);                                               AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc->AddAudioOut (eAlgFieldID_AudioOut);                                             AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc->AddAudioBufferLength (eAlgFieldID_BufferSize);                                  AAX_ASSERT (err == AAX_SUCCESS);
	AAX_ASSERT( eMeterTap_Count == sizeof(cDemoGain_MeterID)/sizeof(AAX_CTypeID) );
	err = outDesc->AddMeters ( eAlgFieldID_Meters, cDemoGain_MeterID, eMeterTap_Count );           AAX_ASSERT (err == AAX_SUCCESS);
    //
	// Register context fields as communications destinations (i.e. input)
	err = outDesc->AddDataInPort ( eAlgPortID_BypassIn, sizeof (int32_t) );                        AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc->AddDataInPort ( eAlgPortID_CoefsGainIn, sizeof (SDemoGain_DMA_CoefsGain) );     AAX_ASSERT (err == AAX_SUCCESS);
	
}

// ***************************************************************************
// ROUTINE:	DescribeAlgorithmComponent_ScatterGather
// DemoGain_DMA Scatter/Gather algorithm component description
// ***************************************************************************
static void DescribeAlgorithmComponent_ScatterGather( AAX_IComponentDescriptor * outDesc )
{
	AAX_Result err = AAX_SUCCESS;
	
	DescribeAlgorithmComponent_Common (outDesc);
	
	// Describe algorithm's context structure
	//
	// Subscribe context fields to host-provided services or information
	err = outDesc->AddDmaInstance (eAlgFieldID_ScatterGather_DmaScatter, AAX_IDma::eMode_Scatter);               AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc->AddDmaInstance (eAlgFieldID_ScatterGather_DmaGather, AAX_IDma::eMode_Gather);                 AAX_ASSERT (err == AAX_SUCCESS);
	//
	// Register private data fields
	// TODO: Create TI- and Native-specific versions of these fields to allocate
	// only the amount of memory required by the min/max buffer sizes of the 
	// platform.  Cannot currently do this because of PTSW-156195
	err = outDesc->AddPrivateData (eAlgFieldID_ScatterGather_DMAAudioBuffer,
								   sizeof(SDemoGain_DMASG_RWBuffer),
								   AAX_ePrivateDataOptions_External);        AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc->AddPrivateData (eAlgFieldID_ScatterGather_DMALocalData,
								   sizeof(SDemoGain_DMASG_DMALocalData));    AAX_ASSERT (err == AAX_SUCCESS);	
	err = outDesc->AddPrivateData (eAlgFieldID_ScatterGather_ScratchBuffer,
								   sizeof(SDemoGain_DMASG_ScratchBuffer));   AAX_ASSERT (err == AAX_SUCCESS);
	
	// Register processing callbacks
	//
	// Create a property map 
	AAX_IPropertyMap* properties = outDesc->NewPropertyMap();
	AAX_ASSERT (properties);
	if ( !properties ) return;
	//
	// Common properties
	DemoGain_DMA_AddCommonAlgProperties (properties);
	//
	// Scatter/Gather properties
	properties->AddProperty ( AAX_eProperty_PlugInID_Native, cDemoGain_DMASG_PlugInID_Native );
	properties->AddProperty ( AAX_eProperty_PlugInID_AudioSuite, cDemoGain_DMASG_PlugInID_AudioSuite );	// Since this is a linear plug-in the Native version can also be an AudioSuite version.
	properties->AddProperty ( AAX_eProperty_PlugInID_TI, cDemoGain_DMASG_PlugInID_TI );
	properties->AddProperty ( AAX_eProperty_TI_MaxInstancesPerChip, 1 ); // For simplicity
	
    // Register callbacks
    //
    // Native
	err = outDesc->AddProcessProc_Native ( DemoGain_DMASG_AlgorithmProcessFunction, properties );
	AAX_ASSERT (err == AAX_SUCCESS);
	//
	// TI
	properties->AddProperty ( AAX_eProperty_DSP_AudioBufferLength, cDemoGain_DMA_TIBufferSizeProperty ); 
	err = outDesc->AddProcessProc_TI ("DemoGain_MM_DMA_Example.dll", "AlgEntry_ScatterGather", properties );
	AAX_ASSERT (err == AAX_SUCCESS);
}

// ***************************************************************************
// ROUTINE:	DescribeAlgorithmComponent_Burst
// DemoGain_DMA Burst algorithm component description
// ***************************************************************************
static void DescribeAlgorithmComponent_Burst( AAX_IComponentDescriptor * outDesc )
{
	AAX_Result err = AAX_SUCCESS;
	
	DescribeAlgorithmComponent_Common (outDesc);
	
	// Describe algorithm's context structure
	//
	// Subscribe context fields to host-provided services or information
	err = outDesc->AddDmaInstance (eAlgFieldID_Burst_DmaBurst, AAX_IDma::eMode_Burst);  AAX_ASSERT (err == AAX_SUCCESS);
	//
	// Register private data fields
	// TODO: Create TI- and Native-specific versions of these fields to allocate
	// only the amount of memory required by the min/max buffer sizes of the 
	// platform.  Cannot currently do this because of PTSW-156195
	err = outDesc->AddPrivateData (eAlgFieldID_Burst_DMAAudioBuffer,
								   sizeof(SDemoGain_DMABurst_RWBuffer),
								   AAX_ePrivateDataOptions_External);        AAX_ASSERT (err == AAX_SUCCESS);
	err = outDesc->AddPrivateData (eAlgFieldID_Burst_DMALocalData,
								   sizeof(SDemoGain_Burst_DMALocalData));    AAX_ASSERT (err == AAX_SUCCESS);	
	
	// Register processing callbacks
	//
	// Create a property map 
	AAX_IPropertyMap* properties = outDesc->NewPropertyMap();
	AAX_ASSERT (properties);
	if ( !properties ) return;
	//
	// Common properties
	DemoGain_DMA_AddCommonAlgProperties (properties);
	//
	// Burst properties
	properties->AddProperty ( AAX_eProperty_PlugInID_Native, cDemoGain_DMABurst_PlugInID_Native );
	properties->AddProperty ( AAX_eProperty_PlugInID_AudioSuite, cDemoGain_DMABurst_PlugInID_AudioSuite );	// Since this is a linear plug-in the Native version can also be an AudioSuite version.
	properties->AddProperty ( AAX_eProperty_PlugInID_TI, cDemoGain_DMABurst_PlugInID_TI );
	properties->AddProperty ( AAX_eProperty_TI_MaxInstancesPerChip, 1 ); // For simplicity
	
    // Register callbacks
    //
    // Native
	err = outDesc->AddProcessProc_Native ( DemoGain_DMABurst_AlgorithmProcessFunction, properties );
	AAX_ASSERT (err == AAX_SUCCESS);
	//
	// TI
	properties->AddProperty ( AAX_eProperty_DSP_AudioBufferLength, cDemoGain_DMA_TIBufferSizeProperty ); 
	err = outDesc->AddProcessProc_TI ("DemoGain_MM_DMA_Example.dll", "AlgEntry_Burst", properties );
	AAX_ASSERT (err == AAX_SUCCESS);
}

// ***************************************************************************
// ROUTINE:	DemoGain_DMA_GetEffectDescription
// Common DemoGain_DMA Effect description
// ***************************************************************************
static AAX_Result DemoGain_DMA_GetEffectDescription( AAX_IEffectDescriptor * outDescriptor )
{
	AAX_Result					err = AAX_SUCCESS;
	
	// Effect identifiers
	//
	outDescriptor->AddName ( "Demo Gain DMA" );
	outDescriptor->AddName ( "Demo Gain" );
	outDescriptor->AddName ( "DemoGain" );
	outDescriptor->AddName ( "DmGain" );
	outDescriptor->AddName ( "DGpr" );
	outDescriptor->AddName ( "Dn" );
	outDescriptor->AddCategory ( AAX_ePlugInCategory_Example );

	// Effect components
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
	
	return err;
}

// ***************************************************************************
// ROUTINE:	DemoGain_DMASG_GetEffectDescription
// DemoGain_DMA Scatter/Gather Effect description
// ***************************************************************************
static AAX_Result DemoGain_DMASG_GetEffectDescription( AAX_IEffectDescriptor * outDescriptor )
{
	AAX_Result					err = AAX_SUCCESS;
	AAX_IComponentDescriptor *	compDesc = outDescriptor->NewComponentDescriptor ();
	if ( !compDesc )
		return AAX_ERROR_NULL_OBJECT;
	
	// Common Effect description
	//
	err = DemoGain_DMA_GetEffectDescription (outDescriptor);
	
	// Effect identifiers
	//
	outDescriptor->AddName ( "Demo Gain (DMA S/G)" );
	
	// Effect components
	//
	// Algorithm component
	DescribeAlgorithmComponent_ScatterGather( compDesc );
	err = outDescriptor->AddComponent( compDesc );
	AAX_ASSERT (err == AAX_SUCCESS);
		
	return err;
}

// ***************************************************************************
// ROUTINE:	DemoGain_DMABurst_GetEffectDescription
// DemoGain_DMA Burst Effect description
// ***************************************************************************
static AAX_Result DemoGain_DMABurst_GetEffectDescription( AAX_IEffectDescriptor * outDescriptor )
{
	AAX_Result					err = AAX_SUCCESS;
	AAX_IComponentDescriptor *	compDesc = outDescriptor->NewComponentDescriptor ();
	if ( !compDesc )
		return AAX_ERROR_NULL_OBJECT;
	
	// Common Effect description
	//
	err = DemoGain_DMA_GetEffectDescription (outDescriptor);
	
	// Effect identifiers
	//
	outDescriptor->AddName ( "Demo Gain (DMA Burst)" );
	
	// Effect components
	//
	// Algorithm component
	DescribeAlgorithmComponent_Burst( compDesc );
	err = outDescriptor->AddComponent( compDesc );
	AAX_ASSERT (err == AAX_SUCCESS);

	return err;
}

// ***************************************************************************
// ROUTINE:	GetEffectDescriptions
// ***************************************************************************
AAX_Result GetEffectDescriptions( AAX_ICollection * outCollection )
{
	AAX_Result				result = AAX_SUCCESS;
	
	AAX_IEffectDescriptor *	plugInDescriptor_DMASG = outCollection->NewDescriptor();
	if ( plugInDescriptor_DMASG && result == AAX_SUCCESS )
	{
		result = DemoGain_DMASG_GetEffectDescription( plugInDescriptor_DMASG );
		if ( result == AAX_SUCCESS )
			result = outCollection->AddEffect( kEffectID_DemoGain_DMASG, plugInDescriptor_DMASG );
		AAX_ASSERT (result == AAX_SUCCESS);
	}
	else result = AAX_ERROR_NULL_OBJECT;
	
	AAX_IEffectDescriptor *	plugInDescriptor_DMABurst = outCollection->NewDescriptor();
	if ( plugInDescriptor_DMABurst && result == AAX_SUCCESS )
	{
		result = DemoGain_DMABurst_GetEffectDescription( plugInDescriptor_DMABurst );
		if ( result == AAX_SUCCESS )
			result = outCollection->AddEffect( kEffectID_DemoGain_DMABurst, plugInDescriptor_DMABurst );
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
