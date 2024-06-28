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

#include "AAX_VCollection.h"
#include "AAX_VEffectDescriptor.h"
#include "AAX_VPropertyMap.h"

#include "AAX_UIDs.h"
#include "acfbaseapi.h"

// ******************************************************************************************
// METHOD:	AAX_VCollection
// ******************************************************************************************
AAX_VCollection::AAX_VCollection (IACFUnknown * pUnkHost) :
	mUnkHost( pUnkHost ),
	mIACFCollection(NULL)
{
	if ( mUnkHost )
	{
		// Get the component factory service from the host so we can create the 
		// built-in plug-in definition.
		ACFPtr<IACFComponentFactory> pFactory;
		if ( pUnkHost->QueryInterface(IID_IACFComponentFactory, (void **)&pFactory) == ACF_OK )
		{
			// Create the object and get the base interface for it.
			pFactory->CreateComponent(AAXCompID_AAXCollection, 0, IID_IAAXCollectionV1, (void **)&mIACFCollection);

			// Try to get later versions of the interface as necessary.
		}
	}
}

// ******************************************************************************************
// METHOD:	~AAX_VCollection
// ******************************************************************************************
AAX_VCollection::~AAX_VCollection ()
{
	std::set<AAX_IEffectDescriptor *>::iterator iter = mEffectDescriptors.begin ();
	for ( ; iter != mEffectDescriptors.end (); ++iter )
		delete *iter;
}

// ******************************************************************************************
// METHOD:	GetIUnknown
// ******************************************************************************************
IACFPluginDefinition*	
AAX_VCollection::GetIUnknown(void) const
{
	return mIACFCollection;
}

// ******************************************************************************************
// METHOD:	NewDescriptor
// ******************************************************************************************
AAX_IEffectDescriptor * AAX_VCollection::NewDescriptor ()
{
	AAX_VEffectDescriptor * plugInDescriptor = new AAX_VEffectDescriptor( mUnkHost );
	mEffectDescriptors.insert( plugInDescriptor );
	return plugInDescriptor;
}


// ******************************************************************************************
// METHOD:	AddEffect
// ******************************************************************************************
AAX_Result AAX_VCollection::AddEffect ( const char * inEffectID, AAX_IEffectDescriptor * inEffectDescriptor )
{
	if ( mIACFCollection && inEffectDescriptor )
		return mIACFCollection->AddEffect( inEffectID, static_cast<AAX_VEffectDescriptor*>(inEffectDescriptor)->GetIUnknown() );
		
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	SetManufacturerName
// ******************************************************************************************
AAX_Result AAX_VCollection::SetManufacturerName( const char * iManufacturerName )
{
	if ( mIACFCollection )
		return mIACFCollection->SetManufacturerName( iManufacturerName );
		
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddPackageName
// ******************************************************************************************
AAX_Result AAX_VCollection::AddPackageName( const char * iPackageName )
{
	if ( mIACFCollection )
		return mIACFCollection->AddPackageName( iPackageName );
		
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	SetPackageVersion
// ******************************************************************************************
AAX_Result AAX_VCollection::SetPackageVersion( uint32_t iPackageVersion )
{
	if ( mIACFCollection )
		return mIACFCollection->SetPackageVersion( iPackageVersion );
		
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	NewPropertyMap
// ******************************************************************************************
AAX_IPropertyMap * AAX_VCollection::NewPropertyMap ()
{
	AAX_VPropertyMap * propertyMap = new AAX_VPropertyMap( mUnkHost );
	mPropertyMaps.insert( propertyMap );
	return propertyMap;	
}

// ******************************************************************************************
// METHOD:	SetProperties
// ******************************************************************************************
AAX_Result AAX_VCollection::SetProperties ( AAX_IPropertyMap * inProperties )
{
	if ( mIACFCollection )
		return mIACFCollection->SetProperties( inProperties ? static_cast<AAX_VPropertyMap*>(inProperties)->GetIUnknown() : NULL);

	return AAX_ERROR_NULL_OBJECT;
}

