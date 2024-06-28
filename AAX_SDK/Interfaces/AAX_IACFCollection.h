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
 *	\file  AAX_IACFCollection.h
 *
 *	\brief Versioned interface to represent a plug-in binary's static description
 *
 */ 
/*================================================================================================*/


#ifndef AAX_IACFCOLLECTION_H
#define AAX_IACFCOLLECTION_H

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

#include "acfbaseapi.h"

class AAX_IEffectDescriptor;

/**	\brief Versioned interface to represent a plug-in binary's static description
 */
class AAX_IACFCollection : public IACFPluginDefinition
{
public:
	
	virtual	AAX_Result				AddEffect ( const char * iEffectID, IACFUnknown * iEffectDescriptor ) = 0;	///< \copydoc AAX_ICollection::AddEffect
	virtual	AAX_Result				SetManufacturerName( const char* iPackageName ) = 0;	///< \copydoc AAX_ICollection::SetManufacturerName()
	virtual	AAX_Result				AddPackageName( const char *iPackageName ) = 0;	///< \copydoc AAX_ICollection::AddPackageName()
	virtual	AAX_Result				SetPackageVersion( uint32_t iVersion ) = 0;	///< \copydoc AAX_ICollection::SetPackageVersion()
	/**	\copydoc AAX_ICollection::SetProperties()
	 *	
	 *	\todo Collection (binary) level properties are not yet implemented in AAE
	 */
	virtual AAX_Result				SetProperties ( IACFUnknown * iProperties ) = 0;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif
