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

#include "AAX_VEffectDescriptor.h"
#include "AAX_VComponentDescriptor.h"
#include "AAX_VPropertyMap.h"

#include "AAX_UIDs.h"
#include "acfbaseapi.h"

// ******************************************************************************************
// METHOD:	AAX_VEffectDescriptor
// ******************************************************************************************
AAX_VEffectDescriptor::AAX_VEffectDescriptor ( IACFUnknown * pUnkHost ) :
	mUnkHost( pUnkHost ),
	mIACFEffectDescriptor( NULL ),
	mIACFEffectDescriptorV2( NULL )
{
	if ( mUnkHost )
	{
		// Get the component factory service from the host so we can create the 
		// built-in plug-in definition.
		ACFPtr<IACFComponentFactory> pFactory;
		if ( pUnkHost->QueryInterface(IID_IACFComponentFactory, (void **)&pFactory) == ACF_OK )
		{
			// Create the object and get the base interface for it.
			pFactory->CreateComponent(AAXCompID_AAXEffectDescriptor, 0, IID_IAAXEffectDescriptorV1, (void **)&mIACFEffectDescriptor);
            if (mIACFEffectDescriptor)
                mIACFEffectDescriptor->QueryInterface(IID_IAAXEffectDescriptorV2, (void**)&mIACFEffectDescriptorV2);
		}
	}
}

// ******************************************************************************************
// METHOD:	~AAX_VEffectDescriptor
// ******************************************************************************************
AAX_VEffectDescriptor::~AAX_VEffectDescriptor ()
{
	std::set<AAX_IComponentDescriptor *>::iterator iterComponentDescriptor = mComponentDescriptors.begin ();
	for ( ; iterComponentDescriptor != mComponentDescriptors.end (); ++iterComponentDescriptor )
		delete *iterComponentDescriptor;

	std::set<AAX_IPropertyMap *>::iterator iterPropertyMap = mPropertyMaps.begin ();
	for ( ; iterPropertyMap != mPropertyMaps.end (); ++iterPropertyMap )
		delete *iterPropertyMap;
}


// ******************************************************************************************
// METHOD:	GetIUnknown
// ******************************************************************************************
IACFUnknown*							
AAX_VEffectDescriptor::GetIUnknown(void) const
{
	return mIACFEffectDescriptor;
}

// ******************************************************************************************
// METHOD:	NewComponentDescriptor
// ******************************************************************************************
AAX_IComponentDescriptor * AAX_VEffectDescriptor::NewComponentDescriptor ()
{
	AAX_VComponentDescriptor * componentDescriptor = new AAX_VComponentDescriptor( mUnkHost );
	mComponentDescriptors.insert( componentDescriptor );
	return componentDescriptor;
}

// ******************************************************************************************
// METHOD:	AddComponent
// ******************************************************************************************
AAX_Result AAX_VEffectDescriptor::AddComponent ( AAX_IComponentDescriptor * inComponentDescriptor )
{
	if ( mIACFEffectDescriptor )
		return mIACFEffectDescriptor->AddComponent( inComponentDescriptor ? static_cast<AAX_VComponentDescriptor*>(inComponentDescriptor)->GetIUnknown() : NULL );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddName
// ******************************************************************************************
AAX_Result AAX_VEffectDescriptor::AddName( const char * iPlugInName )
{
	if ( mIACFEffectDescriptor )
		return mIACFEffectDescriptor->AddName( iPlugInName );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddCategory
// ******************************************************************************************
AAX_Result AAX_VEffectDescriptor::AddCategory( uint32_t iCategory )
{
	if ( mIACFEffectDescriptor )
		return mIACFEffectDescriptor->AddCategory( iCategory );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddCategoryBypassParameter
// ******************************************************************************************
AAX_Result AAX_VEffectDescriptor::AddCategoryBypassParameter ( uint32_t iCategory, AAX_CParamID inParamID )
{
	if ( mIACFEffectDescriptor )
		return mIACFEffectDescriptor->AddCategoryBypassParameter ( iCategory, inParamID );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddProcPtr
// ******************************************************************************************
AAX_Result AAX_VEffectDescriptor::AddProcPtr( void * iProcPtr, AAX_CProcPtrID iProcID )
{
	if ( mIACFEffectDescriptor )
		return mIACFEffectDescriptor->AddProcPtr( iProcPtr, iProcID );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	NewPropertyMap
// ******************************************************************************************
AAX_IPropertyMap * AAX_VEffectDescriptor::NewPropertyMap ()
{
	AAX_VPropertyMap * propertyMap = new AAX_VPropertyMap( mUnkHost );
	mPropertyMaps.insert( propertyMap );
	return propertyMap;	
}

// ******************************************************************************************
// METHOD:	SetProperties
// ******************************************************************************************
AAX_Result AAX_VEffectDescriptor::SetProperties ( AAX_IPropertyMap * inProperties )
{
	if ( mIACFEffectDescriptor )
		return mIACFEffectDescriptor->SetProperties( inProperties ? static_cast<AAX_VPropertyMap*>(inProperties)->GetIUnknown() : NULL);

	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddResourceInfo
// ******************************************************************************************
AAX_Result AAX_VEffectDescriptor::AddResourceInfo ( AAX_EResourceType inResourceType, const char * inFileName )
{
	if ( mIACFEffectDescriptor )
		return mIACFEffectDescriptor->AddResourceInfo ( inResourceType, inFileName );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddMeterDescription
// ******************************************************************************************
AAX_Result AAX_VEffectDescriptor::AddMeterDescription( AAX_CTypeID inMeterID, const char * inMeterName, AAX_IPropertyMap * inProperties )
{
	if ( mIACFEffectDescriptor )
	{
		IACFUnknown* propMap = inProperties ? static_cast<AAX_VPropertyMap*>(inProperties)->GetIUnknown() : NULL;
		return mIACFEffectDescriptor->AddMeterDescription( inMeterID, inMeterName, propMap );
	}

	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddControlMIDINode
// ******************************************************************************************
AAX_Result AAX_VEffectDescriptor::AddControlMIDINode ( AAX_CTypeID iNodeID, AAX_EMIDINodeType iNodeType, const char iNodeName[], uint32_t channelMask )
{
	if ( mIACFEffectDescriptorV2 )
		return mIACFEffectDescriptorV2->AddControlMIDINode( iNodeID, iNodeType, iNodeName, channelMask );

	return AAX_ERROR_UNIMPLEMENTED;
}
