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
 *	\file   AAX_IPropertyMap.h
 *	
 *	\brief	Generic plug-in description property map
 *
 */ 
/*================================================================================================*/


#ifndef AAX_IPROPERTYMAP_H
#define AAX_IPROPERTYMAP_H

#include "AAX_Properties.h"
#include "AAX.h"

/**
 	\brief Generic plug-in description property map
 	
 	\details
 	\hostimp
 	
	Property Maps are used to associate specific sets of properties with plug-in description
	interfaces.  For example, an audio processing component might register mono and stereo callbacks,
	or Native and TI callbacks, assigning each \c ProcessProc the applicable property mapping.
	This allows the host to determine the correct callback to use depending on the environment in
	which the plug-in is instantiated.
		
	%AAX does not require that every value in %AAX IPropertyMap be assigned by the developer.
	Unassigned properties do not have defined default values; if a
	specific value is not assigned to one of an element's properties then the element must 
	support any value for that property.  For example, if an audio processing component does
	not define its callback's audio buffer length property, the host will assume that the callback
	will support any buffer length.
 
	- To create a new property map: \ref AAX_IComponentDescriptor::NewPropertyMap()
	- To copy an existing property map: \ref AAX_IComponentDescriptor::DuplicatePropertyMap()
 
 	\ingroup CommonInterface_Describe
 
 */
class AAX_IPropertyMap 
{
public:
	virtual ~AAX_IPropertyMap() {}
	
	/** @brief	Get a property value from a property map.
			
			Returns true if the selected property is supported, false if it is not
			
			@param[in]	iProperty
						The property ID
			@param[in]	oValue
						The property value
			
			
	*/
	virtual AAX_CBoolean		GetProperty ( AAX_EProperty iProperty, AAX_CPropertyValue * oValue ) const = 0;
	/** @brief	Add a callback property to a property map
			
			@param[in]	iProperty
						The property ID.
			@param[in]	iValue
						
			
	*/
	virtual AAX_Result			AddProperty ( AAX_EProperty iProperty, AAX_CPropertyValue iValue ) = 0;
	/** @brief	Remove a callback property from a property map
			
			@param[in]	iProperty	
						The property ID.
	*/
	virtual AAX_Result			RemoveProperty ( AAX_EProperty iProperty ) = 0;
    
	/** @brief	Add an array of plug-in IDs to a property map
	 
			 @param[in]	iProperty
						The property ID.
			 @param[in]	iPluginIDs
						An array of \ref AAX_SPlugInIdentifierTriad
			 @param[in]	iNumPluginIDs
						The length of \c iPluginIDs
	 
	 */
    virtual AAX_Result          AddPropertyWithIDArray ( AAX_EProperty iProperty, const AAX_SPlugInIdentifierTriad* iPluginIDs, uint32_t iNumPluginIDs) = 0;
    
	/** @brief	Get an array of plug-in IDs from a property map
	 
			 @param[in]		iProperty
							The property ID.
			 @param[out]	oPluginIDs
							A pointer that will be set to reference an array of \ref AAX_SPlugInIdentifierTriad
			 @param[in]		oNumPluginIDs
							The length of \c oPluginIDs
	 
	 */
    virtual AAX_CBoolean        GetPropertyWithIDArray ( AAX_EProperty iProperty, const AAX_SPlugInIdentifierTriad** oPluginIDs, uint32_t* oNumPluginIDs) const = 0;

};

#endif // AAX_IPROPERTYMAP_H
