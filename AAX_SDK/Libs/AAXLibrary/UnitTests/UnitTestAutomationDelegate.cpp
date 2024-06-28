/*================================================================================================*/
/*
 *	Copyright 2011-2015 by Avid Technology, Inc.
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

#include "UnitTestAutomationDelegate.h"
#include "AAX_CEffectParameters.h"				//For NormalizedToInt32()
#include "AAX_UIDs.h"

ACFRESULT UnitTestAutomationDelegate::InternalQueryInterface(const acfIID & riid, void **ppvObjOut)
{
	if (riid == IID_IAAXAutomationDelegateV1 )
	{ 
		*ppvObjOut = static_cast<IACFUnknown *>(this);
		( static_cast<IACFUnknown *>(*ppvObjOut))->AddRef();
		return ACF_OK;
	}
	
	return this->CACFUnknown::InternalQueryInterface(riid, ppvObjOut);
}
	
	
AAX_Result	UnitTestAutomationDelegate::PostSetValueRequest( AAX_CParamID controlIdentifier, double normalizedValue ) const	
{  
	mEffectParameters->UpdateParameterNormalizedValue(controlIdentifier, normalizedValue, AAX_eUpdateSource_Unspecified); // TODO: Not sure if "unspecified" source is correct here	
	return AAX_SUCCESS;
}
