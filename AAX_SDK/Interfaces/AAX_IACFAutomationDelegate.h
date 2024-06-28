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
 *	\file  AAX_IACFAutomationDelegate.h
 *
 *	\brief Versioned interface allowing an %AAX plug-in to interact with the host's automation system
 *
 */ 
/*================================================================================================*/


#ifndef AAX_IACFAUTOMATIONDELEGATE_H
#define AAX_IACFAUTOMATIONDELEGATE_H

#include "AAX.h"

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#endif

#include "acfunknown.h"

/**	\brief Versioned interface allowing an %AAX plug-in to interact with the host's automation system
	
	\details
	\sa \ref advancedTopics_ParameterUpdates
 */
class AAX_IACFAutomationDelegate : public IACFUnknown
{
public:
	
	/**	\brief Register a control with the automation system using a char* based control identifier
	*	
	*	\details
	*	These identifiers are consistent with Parameter Mananger control IDs.
	*/
	virtual AAX_Result	RegisterParameter ( AAX_CParamID iParameterID ) = 0;

	/**	\brief Unregister a control with the automation system using a char* based control identifier
	*	
	*	\details
	*	These identifiers are consistent with Parameter Mananger control IDs.  All registered controls
	*	should be unregistered or the system might leak.
	*/
	virtual AAX_Result	UnregisterParameter ( AAX_CParamID iParameterID ) = 0;


	/**	\brief Sends a Set token out to request that a value be changed
	*   
	*	\details
	*	normalized value is a double [0 1]
	*/
	virtual AAX_Result	PostSetValueRequest ( AAX_CParamID iParameterID, double normalizedValue ) const = 0;

	/**	\brief Sends a Current Value token out to notify everyone that a value has changed
	*   
	*	\details
	*	normalized value is a double [0 1]
	*/
	virtual AAX_Result	PostCurrentValue( AAX_CParamID iParameterID, double normalizedValue ) const = 0;
	
	/**	\brief Sends a Touch token out.
	*/
	virtual AAX_Result	PostTouchRequest( AAX_CParamID iParameterID ) = 0;
	
	/**	\brief Sends a Release token out.
	 */
	virtual AAX_Result	PostReleaseRequest( AAX_CParamID iParameterID ) = 0;
	
	/**	\brief Gets the touched state of the parameter.
	 */
	virtual AAX_Result	GetTouchState ( AAX_CParamID iParameterID, AAX_CBoolean * oTouched )= 0;
};

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#endif // AAX_IACFAUTOMATIONDELEGATE_H
