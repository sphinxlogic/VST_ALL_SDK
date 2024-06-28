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
 *
 */
/*================================================================================================*/

#include "AAX_VComponentDescriptor.h"
#include "AAX_VPropertyMap.h"

#include "AAX_UIDs.h"
#include "acfbaseapi.h"
#include "ACFPtr.h"

// ******************************************************************************************
// METHOD:	AAX_VComponentDescriptor
// ******************************************************************************************
AAX_VComponentDescriptor::AAX_VComponentDescriptor( IACFUnknown * pUnkHost ) :
	mUnkHost( pUnkHost ),
	mIACFComponentDescriptor( NULL ),
    mIACFComponentDescriptorV2( NULL )
{
	if ( mUnkHost )
	{
		// Get the component factory service from the host so we can create the
		// built-in plug-in definition.
		ACFPtr<IACFComponentFactory> pFactory;
		if ( pUnkHost->QueryInterface(IID_IACFComponentFactory, (void **)&pFactory) == ACF_OK )
		{
        
            // Create the object and get the base interface for it.
			pFactory->CreateComponent(AAXCompID_AAXComponentDescriptor, 0, IID_IAAXComponentDescriptorV1, (void **)&mIACFComponentDescriptor);
            if (mIACFComponentDescriptor)
                mIACFComponentDescriptor->QueryInterface(IID_IAAXComponentDescriptorV2, (void**)&mIACFComponentDescriptorV2);
		}
	}
}

// ******************************************************************************************
// METHOD:	~AAX_VComponentDescriptor
// ******************************************************************************************
AAX_VComponentDescriptor::~AAX_VComponentDescriptor ()
{
	std::set<AAX_IPropertyMap *>::iterator iter = mPropertyMaps.begin ();
	for ( ; iter != mPropertyMaps.end (); ++iter )
		delete *iter;
}


// ******************************************************************************************
// METHOD:	GetIUnknown
// ******************************************************************************************
IACFUnknown*				
AAX_VComponentDescriptor::GetIUnknown(void) const
{
	return mIACFComponentDescriptor;
}

// ******************************************************************************************
// METHOD:	Clear
// ******************************************************************************************
AAX_Result AAX_VComponentDescriptor::Clear ()
{
	if ( mIACFComponentDescriptor ) 
		return mIACFComponentDescriptor->Clear();
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddReservedField
// ******************************************************************************************
AAX_Result AAX_VComponentDescriptor::AddReservedField ( AAX_CFieldIndex inPortID, uint32_t inFieldType )
{
	if ( mIACFComponentDescriptor ) 
		return mIACFComponentDescriptor->AddReservedField ( inPortID, inFieldType );

	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddAudioIn
// ******************************************************************************************
AAX_Result AAX_VComponentDescriptor::AddAudioIn ( AAX_CFieldIndex inPortID )
{
	if ( mIACFComponentDescriptor ) 
		return mIACFComponentDescriptor->AddAudioIn ( inPortID );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddAudioOut
// ******************************************************************************************
AAX_Result AAX_VComponentDescriptor::AddAudioOut ( AAX_CFieldIndex inPortID )
{
	if ( mIACFComponentDescriptor ) 
		return mIACFComponentDescriptor->AddAudioOut ( inPortID );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddAudioBufferLength
// ******************************************************************************************
AAX_Result AAX_VComponentDescriptor::AddAudioBufferLength ( AAX_CFieldIndex inPortID )
{
	if ( mIACFComponentDescriptor ) 
		return mIACFComponentDescriptor->AddAudioBufferLength ( inPortID );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddSampleRate
// ******************************************************************************************
AAX_Result AAX_VComponentDescriptor::AddSampleRate ( AAX_CFieldIndex inPortID )
{
	if ( mIACFComponentDescriptor ) 
		return mIACFComponentDescriptor->AddSampleRate ( inPortID );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddClock
// ******************************************************************************************
AAX_Result AAX_VComponentDescriptor::AddClock ( AAX_CFieldIndex inPortID )
{
	if ( mIACFComponentDescriptor ) 
		return mIACFComponentDescriptor->AddClock ( inPortID );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddSideChainIn
// ******************************************************************************************
AAX_Result AAX_VComponentDescriptor::AddSideChainIn ( AAX_CFieldIndex inPortID )
{
	if ( mIACFComponentDescriptor ) 
		return mIACFComponentDescriptor->AddSideChainIn ( inPortID );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddDataInPort
// ******************************************************************************************
AAX_Result AAX_VComponentDescriptor::AddDataInPort ( AAX_CFieldIndex inPortID, uint32_t inPacketSize, AAX_EDataInPortType inPortType )
{
	if ( mIACFComponentDescriptor ) 
		return mIACFComponentDescriptor->AddDataInPort ( inPortID, inPacketSize, inPortType);
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddAuxOutputStem
// ******************************************************************************************
AAX_Result AAX_VComponentDescriptor::AddAuxOutputStem ( AAX_CFieldIndex inPortID, int32_t inStemFormat, const char inNameUTF8[] )
{
	if ( mIACFComponentDescriptor ) 
		return mIACFComponentDescriptor->AddAuxOutputStem ( inPortID, inStemFormat, inNameUTF8 );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddPrivateData
// ******************************************************************************************
AAX_Result AAX_VComponentDescriptor::AddPrivateData ( AAX_CFieldIndex inPortID, int32_t inDataSize, uint32_t /* AAX_EPrivateDataOptions */ inOptions /* = AAX_ePrivateDataOptions_DefaultOptions */ )
{
	if ( mIACFComponentDescriptor ) 
		return mIACFComponentDescriptor->AddPrivateData ( inPortID, inDataSize, inOptions );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddDmaInstance
// ******************************************************************************************
AAX_Result AAX_VComponentDescriptor::AddDmaInstance ( AAX_CFieldIndex inPortID, AAX_IDma::EMode inDmaMode )
{
	if ( mIACFComponentDescriptor ) 
		return mIACFComponentDescriptor->AddDmaInstance ( inPortID, inDmaMode );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// ******************************************************************************************
// METHOD:	AddMeter
// ******************************************************************************************
AAX_Result AAX_VComponentDescriptor::AddMeters (
												AAX_CFieldIndex					inPortID,
												const AAX_CTypeID*			inMeterIDs,
												const uint32_t				inMeterCount)
{
	if ( mIACFComponentDescriptor ) 
		return mIACFComponentDescriptor->AddMeters ( inPortID, inMeterIDs, inMeterCount );

	return AAX_ERROR_NULL_OBJECT;
}

// METHOD:	NewPropertyMap
// ******************************************************************************************
AAX_IPropertyMap * AAX_VComponentDescriptor::NewPropertyMap () const
{
	AAX_VPropertyMap * propertyMap = new AAX_VPropertyMap( mUnkHost );
	((AAX_VComponentDescriptor *) this)->mPropertyMaps.insert( propertyMap );
	return propertyMap;
}

// METHOD:	ClonePropertyMap
// ******************************************************************************************
AAX_IPropertyMap * AAX_VComponentDescriptor::DuplicatePropertyMap (AAX_IPropertyMap* inPropertyMap) const
{
	AAX_IPropertyMap * newPropertyMap = this->NewPropertyMap();
	for (AAX_EProperty curPropertyID = AAX_eProperty_MinProp;
		 curPropertyID < AAX_eProperty_MaxProp;
		 curPropertyID = (AAX_EProperty((int32_t)curPropertyID + 1)))
	{
		AAX_CPropertyValue curPropertyValue;
		if (inPropertyMap->GetProperty(curPropertyID, &curPropertyValue))
		{
			newPropertyMap->AddProperty(curPropertyID, curPropertyValue);
		}
	}
	
	return newPropertyMap;
}

// ******************************************************************************************
// METHOD:	AddProcessProc_Native
// ******************************************************************************************
AAX_Result AAX_VComponentDescriptor::AddProcessProc_Native( 
	AAX_CProcessProc inProcessProc, 
	AAX_IPropertyMap * inProperties /*= NULL*/, 
	AAX_CInstanceInitProc inInstanceInitProc /*= NULL*/, 
	AAX_CBackgroundProc inBackgroundProc /*= NULL*/, 
	AAX_CSelector * outProcID /*= NULL */ )
{
	if ( mIACFComponentDescriptor )
		return mIACFComponentDescriptor->AddProcessProc_Native( 
			inProcessProc, 
			inProperties ? static_cast<AAX_VPropertyMap*>(inProperties)->GetIUnknown() : NULL, 
			inInstanceInitProc, 
			inBackgroundProc, 
			outProcID );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddProcessProc_TI
// ******************************************************************************************
AAX_Result AAX_VComponentDescriptor::AddProcessProc_TI( 
	const char inDLLFileNameUTF8[], 
	const char inProcessProcSymbol[],
	AAX_IPropertyMap * inProperties /*= NULL*/, 
	const char inInstanceInitProcSymbol [] /*= NULL*/, 
	const char inBackgroundProcSymbol [] /*= NULL*/, 
	AAX_CSelector * outProcID /*= NULL */ )
{
	if ( mIACFComponentDescriptor ) 
		return mIACFComponentDescriptor->AddProcessProc_TI( 
			inDLLFileNameUTF8, 
			inProcessProcSymbol, 
			inProperties ? static_cast<AAX_VPropertyMap*>(inProperties)->GetIUnknown() : NULL, 
			inInstanceInitProcSymbol, 
			inBackgroundProcSymbol, 
			outProcID );
		
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddMIDINode
// ******************************************************************************************
AAX_Result AAX_VComponentDescriptor::AddMIDINode ( AAX_CFieldIndex inPortID, AAX_EMIDINodeType inNodeType, const char inNodeName[], uint32_t channelMask )
{
	if ( mIACFComponentDescriptor ) 
		return mIACFComponentDescriptor->AddMIDINode ( inPortID, inNodeType, inNodeName, channelMask );

	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddTemporaryData
// ******************************************************************************************
AAX_Result AAX_VComponentDescriptor::AddTemporaryData( AAX_CFieldIndex iFieldIndex, uint32_t iDataElementSize)
{
    if (mIACFComponentDescriptorV2)
        return mIACFComponentDescriptorV2->AddTemporaryData(iFieldIndex, iDataElementSize);
    return AAX_ERROR_UNIMPLEMENTED;
}

