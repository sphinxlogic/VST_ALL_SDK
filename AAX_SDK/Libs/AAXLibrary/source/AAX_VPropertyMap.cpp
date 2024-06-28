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

#include "AAX_VPropertyMap.h"
#include "AAX_UIDs.h"
#include "acfbaseapi.h"

// ******************************************************************************************
// METHOD:	AAX_VPropertyMap
// ******************************************************************************************
AAX_VPropertyMap::AAX_VPropertyMap( IACFUnknown * pUnkHost ) :
	mIACFPropertyMap( NULL ),
    mIACFPropertyMapV2( NULL )
{
	if ( pUnkHost )
	{
		// Get the component factory service from the host so we can create the 
		// built-in plug-in definition.
		ACFPtr<IACFComponentFactory> pFactory;
		if ( pUnkHost->QueryInterface(IID_IACFComponentFactory, (void **)&pFactory) == ACF_OK )
		{
			// Create the object and get the base interface for it.
			pFactory->CreateComponent(AAXCompID_AAXPropertyMap, 0, IID_IAAXPropertyMapV1, (void **)&mIACFPropertyMap);
            if (mIACFPropertyMap)
                mIACFPropertyMap->QueryInterface(IID_IAAXPropertyMapV2, (void **)&mIACFPropertyMapV2);
		}
	}
}

// ******************************************************************************************
// METHOD:	~AAX_VPropertyMap
// ******************************************************************************************
AAX_VPropertyMap::~AAX_VPropertyMap(void)
{
}

// ******************************************************************************************
// METHOD:	GetIUnknown
// ******************************************************************************************
IACFUnknown * AAX_VPropertyMap::GetIUnknown () const
{
	return  mIACFPropertyMap;
}

// ******************************************************************************************
// METHOD:	GetProperty
// ******************************************************************************************
AAX_CBoolean AAX_VPropertyMap::GetProperty ( AAX_EProperty inProperty, AAX_CPropertyValue * outValue ) const
{
	AAX_CBoolean	result = 0;
	
	if ( mIACFPropertyMap )
		return mIACFPropertyMap->GetProperty ( inProperty, outValue );
	
	return result;
}

// ******************************************************************************************
// METHOD:	AddProperty
// ******************************************************************************************
AAX_Result AAX_VPropertyMap::AddProperty ( AAX_EProperty inProperty, AAX_CPropertyValue inValue )
{
	if ( mIACFPropertyMap )
		return mIACFPropertyMap->AddProperty ( inProperty, inValue );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	RemoveProperty
// ******************************************************************************************
AAX_Result AAX_VPropertyMap::RemoveProperty ( AAX_EProperty inProperty )
{
	if ( mIACFPropertyMap )
		return mIACFPropertyMap->RemoveProperty ( inProperty );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	AddPropertyWithIDArray
// ******************************************************************************************
AAX_Result AAX_VPropertyMap::AddPropertyWithIDArray ( AAX_EProperty iProperty, const AAX_SPlugInIdentifierTriad* iPluginIDs, uint32_t iNumPluginIDs)
{
    if (mIACFPropertyMapV2)
        return mIACFPropertyMapV2->AddPropertyWithIDArray(iProperty, iPluginIDs, iNumPluginIDs);
    return AAX_ERROR_UNIMPLEMENTED;
}

// ******************************************************************************************
// METHOD:	GetPropertyWithIDArray
// ******************************************************************************************
AAX_CBoolean AAX_VPropertyMap::GetPropertyWithIDArray ( AAX_EProperty iProperty, const AAX_SPlugInIdentifierTriad** oPluginIDs, uint32_t* oNumPluginIDs) const
{
	AAX_CBoolean	result = 0;

    if (mIACFPropertyMapV2)
        return mIACFPropertyMapV2->GetPropertyWithIDArray(iProperty, oPluginIDs, oNumPluginIDs);

    return result;
}

