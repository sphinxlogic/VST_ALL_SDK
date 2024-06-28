/*================================================================================================*/
/*
 *
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

/**  
 *	\file  AAX_VPropertyMap.h
 *
 *	\brief Version-managed concrete PropertyMap class
 *
 */ 
/*================================================================================================*/

#ifndef AAX_VPROPERTYMAP_H
#define AAX_VPROPERTYMAP_H

#include "AAX.h"
#include "AAX_IPropertyMap.h"
#include "AAX_IACFPropertyMap.h"
#include "acfunknown.h"
#include "ACFPtr.h"


class IACFUnknown;
class AAX_IACFPropertyMap;

/**
 *	\brief Version-managed concrete \ref AAX_IPropertyMap class
 *
 */
class AAX_VPropertyMap : public AAX_IPropertyMap
{
public:
	AAX_VPropertyMap ( IACFUnknown * pUnkHost );
	virtual ~AAX_VPropertyMap(void);

	virtual AAX_CBoolean		GetProperty ( AAX_EProperty inProperty, AAX_CPropertyValue * outValue ) const;
	virtual AAX_Result			AddProperty ( AAX_EProperty inProperty, AAX_CPropertyValue inValue );
	virtual AAX_Result			RemoveProperty ( AAX_EProperty inProperty );
    virtual AAX_Result          AddPropertyWithIDArray ( AAX_EProperty iProperty, const AAX_SPlugInIdentifierTriad* iPluginIDs, uint32_t iNumPluginIDs);
    virtual AAX_CBoolean        GetPropertyWithIDArray ( AAX_EProperty iProperty, const AAX_SPlugInIdentifierTriad** oPluginIDs, uint32_t* oNumPluginIDs) const;

	IACFUnknown*				GetIUnknown() const;

private:
	ACFPtr<AAX_IACFPropertyMap>	mIACFPropertyMap;
    ACFPtr<AAX_IACFPropertyMap_V2>	mIACFPropertyMapV2;
};



#endif // AAX_VPROPERTYMAP_H
