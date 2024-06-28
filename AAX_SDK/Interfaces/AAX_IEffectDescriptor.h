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
 *	\file   AAX_IEffectDescriptor.h
 *
 *	\brief	Description interface for an effect's (plug-in type's) components
 *
 */ 
/*================================================================================================*/


#ifndef AAX_IEFFECTDESCRIPTOR_H
#define AAX_IEFFECTDESCRIPTOR_H

#include "AAX.h"
#include "AAX_Callbacks.h"

class AAX_IComponentDescriptor;
class AAX_IPropertyMap;

/**
	\brief	Description interface for an effect's (plug-in type's) components
	
	\details
	\hostimp
 
	Each Effect represents a different "type" of plug-in. The host will present different
	Effects to the user as separate products, even if they are derived from the same
	\ref AAX_ICollection description.
	
	\sa \ref AAX_ICollection::AddEffect()
	
 	\ingroup CommonInterface_Describe
 
 */
class AAX_IEffectDescriptor
{
public:
	virtual ~AAX_IEffectDescriptor() {}
	/** @brief	Create an instance of a component descriptor.
			
	*/
	virtual AAX_IComponentDescriptor *			NewComponentDescriptor () = 0;
	/** @brief	Add a component to an instance of a component descriptor.
			
			Unlike with \ref AAX_ICollection::AddEffect(), the \ref AAX_IEffectDescriptor does
			not take ownership of the \ref AAX_IComponentDescriptor that is passed to it in this
			method. The host copies out the contents of this descriptor, and thus the plug-in may
			re-use the same descriptor object when creating additional similar components.
	 
			@param[in]	iComponentDescriptor	
			
	*/
	virtual AAX_Result							AddComponent ( AAX_IComponentDescriptor* iComponentDescriptor ) = 0;
	/** @brief	Add a name to the Effect.
			
			May be called multiple times to add abbreviated Effect names.
			
			\note Every Effect must include at least one name variant with 31
			or fewer characters, plus a null terminating character
			
			@param[in]	iPlugInName
						The name assigned to the plug-in.
	*/
	virtual AAX_Result							AddName ( const char *iPlugInName ) = 0;
	/** @brief	Add a category to your plug-in. See \ref AAX_EPlugInCategory.
			
			@param[in]	iCategory
						One of the categories for the plug-in.
				
	*/
	virtual AAX_Result							AddCategory ( uint32_t iCategory ) = 0;
	/** @brief	Add a category to your plug-in. See \ref AAX_EPlugInCategory.
	 
			@param[in]	iCategory
						One of the categories for the plug-in.
			@param[in]	iParamID
						The parameter ID of the parameter used to bypass the category seciont of the plug-in.
			 
	 */
	virtual AAX_Result							AddCategoryBypassParameter ( uint32_t iCategory, AAX_CParamID iParamID ) = 0;
	/** @brief	Add a process pointer.
			
			@param[in]	iProcPtr
						A process pointer.
			@param[in]	iProcID
						A process ID.			
			
	*/
	virtual AAX_Result							AddProcPtr ( void * iProcPtr, AAX_CProcPtrID iProcID ) = 0;
	/** @brief	Create a new property map
			
						
	*/
	virtual AAX_IPropertyMap *					NewPropertyMap () = 0;
	/** @brief	Set the properties of a new property map.
	
			@param[in]	iProperties
						Description
		
			
	*/
	virtual AAX_Result							SetProperties ( AAX_IPropertyMap * iProperties ) = 0;
	/** @brief	Set resource file info.
			
			@param[in]	iResourceType
						See AAX_EResourceType.
			@param[in]	iInfo
						Definition varies on the resource type.
			
	*/
	virtual AAX_Result							AddResourceInfo ( AAX_EResourceType iResourceType, const char * iInfo ) = 0;
	/** @brief	Add name and property map to meter with given ID.

			@param[in]	iMeterID
						The ID of the meter being described.
			@param[in]	iMeterName
						The name of the meter.
			@param[in]	iProperties
						The property map containing meter related data such as meter type, orientation, etc.

	*/
	virtual AAX_Result							AddMeterDescription( AAX_CTypeID iMeterID, const char * iMeterName, AAX_IPropertyMap * iProperties ) = 0;
	/** @brief	Add a control MIDI node to the plug-in data model.
			
			- This MIDI node may receive note data as well as control data.
			- To send MIDI data to the plug-in's algorithm, use
			  \ref AAX_IComponentDescriptor::AddMIDINode().
			
			\sa \ref AAX_IACFEffectParameters_V2::UpdateControlMIDINodes()
			
			@param[in]	iNodeID
						The ID for the new control MIDI node.
			@param[in]	iNodeType
						The type of the node.
			@param[in]	iNodeName
						The name of the node.
			@param[in]	iChannelMask
						The bit mask for required nodes channels (up to 16) or required global events for global node.
	*/
	virtual AAX_Result							AddControlMIDINode ( AAX_CTypeID iNodeID, AAX_EMIDINodeType iNodeType, const char iNodeName[], uint32_t iChannelMask ) = 0;

};

#endif // AAX_IEFFECTDESCRIPTOR_H
