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
 *	\file  AAX_IACFEffectDescriptor.h
 *
 *	\brief Versioned interface for an AAX_IEffectDescriptor
 *
 */ 
/*================================================================================================*/


#ifndef AAX_IACFEFFECTDESCRIPTOR_H
#define AAX_IACFEFFECTDESCRIPTOR_H

#include "AAX.h"
#include "AAX_Callbacks.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

#include "acfunknown.h"


/**	\brief Versioned interface for an AAX_IEffectDescriptor
 */ 
class AAX_IACFEffectDescriptor : public IACFUnknown
{
public:
	
	virtual AAX_Result							AddComponent ( IACFUnknown* iComponentDescriptor ) = 0;	///< \copydoc AAX_IEffectDescriptor::AddComponent()
	virtual AAX_Result							AddName ( const char *iPlugInName ) = 0;	///< \copydoc AAX_IEffectDescriptor::AddName()
	virtual AAX_Result							AddCategory ( uint32_t iCategory ) = 0;	///< \copydoc AAX_IEffectDescriptor::AddCategory()
	virtual AAX_Result							AddCategoryBypassParameter ( uint32_t iCategory, AAX_CParamID iParamID ) = 0;	///< \copydoc AAX_IEffectDescriptor::AddCategoryBypassParameter()
	virtual AAX_Result							AddProcPtr ( void * iProcPtr, AAX_CProcPtrID iProcID ) = 0;	///< \copydoc AAX_IEffectDescriptor::AddProcPtr()
	virtual AAX_Result							SetProperties ( IACFUnknown * iProperties ) = 0;	///< \copydoc AAX_IEffectDescriptor::SetProperties()
	virtual AAX_Result							AddResourceInfo ( AAX_EResourceType iResourceType, const char * iInfo ) = 0;	///< \copydoc AAX_IEffectDescriptor::AddResourceInfo()
	virtual AAX_Result							AddMeterDescription( AAX_CTypeID iMeterID, const char * iMeterName, IACFUnknown * iProperties ) = 0;	///< \copydoc AAX_IEffectDescriptor::AddMeterDescription()
};

/**
 *	\brief Versioned interface for an AAX_IEffectDescriptor
 *
 */
class AAX_IACFEffectDescriptor_V2 : public AAX_IACFEffectDescriptor
{
public:
	virtual AAX_Result							AddControlMIDINode ( AAX_CTypeID iNodeID, AAX_EMIDINodeType iNodeType, const char iNodeName[], uint32_t iChannelMask ) = 0;	///< \copydoc AAX_IEffectDescriptor::AddControlMIDINode()
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif // AAX_IACFEFFECTDESCRIPTOR_H
