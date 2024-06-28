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
 *	\file  AAX_VEffectDescriptor.h
 *
 *	\brief Version-managed concrete EffectDescriptor class
 *
 */ 
/*================================================================================================*/

#ifndef AAX_VEFFECTDESCRIPTOR_H
#define AAX_VEFFECTDESCRIPTOR_H

#include "AAX.h"
#include "AAX_IEffectDescriptor.h"
#include "AAX_IACFEffectDescriptor.h"
#include "acfunknown.h"
#include "ACFPtr.h"

#include <set>
#include <map>

class AAX_IComponentDescriptor;
class AAX_IPropertyMap;
class AAX_IACFEffectDescriptor;
class IACFUnknown;

/**
 *	\brief Version-managed concrete \ref AAX_IEffectDescriptor class
 *
 */
class AAX_VEffectDescriptor : public AAX_IEffectDescriptor
{
public:	
	AAX_VEffectDescriptor ( IACFUnknown * pUnkHost );
	virtual ~AAX_VEffectDescriptor ();

	/** \copydoc AAX_IEffectDescriptor::NewComponentDescriptor()
	 *	
	 *	This implementation retains each generated \ref AAX_IComponentDescriptor and destroys the property map upon AAX_VEffectDescriptor destruction
	 */
	virtual AAX_IComponentDescriptor *		NewComponentDescriptor ();
	virtual AAX_Result						AddComponent ( AAX_IComponentDescriptor * inComponentDescriptor );
	virtual AAX_Result						AddName ( const char * inPlugInName );
	virtual AAX_Result						AddCategory ( uint32_t inCategory );
	virtual AAX_Result						AddCategoryBypassParameter ( uint32_t inCategory, AAX_CParamID inParamID );
	virtual AAX_Result						AddProcPtr ( void * inProcPtr, AAX_CProcPtrID inProcID );
	/** \copydoc AAX_IEffectDescriptor::NewPropertyMap()
	 *	
	 *	This implementation retains each generated \ref AAX_IPropertyMap and destroys the property map upon AAX_VEffectDescriptor destruction
	 */
	virtual AAX_IPropertyMap *				NewPropertyMap ();
	virtual AAX_Result						SetProperties ( AAX_IPropertyMap * inProperties );
	virtual AAX_Result						AddResourceInfo ( AAX_EResourceType inResourceType, const char * inInfo );
	virtual AAX_Result						AddMeterDescription( AAX_CTypeID inMeterID, const char * inMeterName, AAX_IPropertyMap * inProperties );
	virtual AAX_Result						AddControlMIDINode ( AAX_CTypeID iNodeID, AAX_EMIDINodeType iNodeType, const char iNodeName[], uint32_t channelMask );

	IACFUnknown*							GetIUnknown(void) const;

private:
	ACFPtr<IACFUnknown>						mUnkHost;
	ACFPtr<AAX_IACFEffectDescriptor>		mIACFEffectDescriptor;
	ACFPtr<AAX_IACFEffectDescriptor_V2>		mIACFEffectDescriptorV2;
	std::set<AAX_IComponentDescriptor *>	mComponentDescriptors;
	std::set<AAX_IPropertyMap *>			mPropertyMaps;

};

#endif // AAX_VEFFECTDESCRIPTOR_H
