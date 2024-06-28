/*================================================================================================*/
/*
 *	Copyright 2010-2015 by Avid Technology, Inc.
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
 *  \file   UnitTestAutomationDelegate.h
 *
 *  \author Dave Tremblay
 *
 */
/*================================================================================================*/

#ifndef UNITTESTAUTOMATIONDELEGATE_H
#define UNITTESTAUTOMATIONDELEGATE_H

#include "AAX_IACFEffectParameters.h"
#include "AAX_IACFAutomationDelegate.h"
#include "CACFUnknown.h"
#include "acfextras.h"
#include "ACFPtr.h"


class UnitTestAutomationDelegate :	public AAX_IACFAutomationDelegate, 
									public CACFUnknown
{
public:	
	//ACF Stuff
	ACF_DECLARE_STANDARD_UNKNOWN()
	ACFMETHOD(InternalQueryInterface)(const acfIID & riid, void **ppvObjOut);
	typedef ACFPtr<UnitTestAutomationDelegate> SmartPtr;
	
	//From IACFAutomationDelegate
	virtual AAX_Result	Initialize(const char* , uint32_t )							{	return AAX_SUCCESS; }
	virtual AAX_Result	Uninitialize()																		{   return AAX_SUCCESS; }
	virtual AAX_Result	RegisterParameter( AAX_CParamID )									{   return AAX_SUCCESS; }
	virtual AAX_Result	UnregisterParameter( AAX_CParamID )									{   return AAX_SUCCESS; }
	virtual AAX_Result	PostCurrentValue( AAX_CParamID , double ) const		{	return AAX_SUCCESS; }
	virtual AAX_Result	PostTouchRequest( AAX_CParamID  )		{	return AAX_SUCCESS; }
	virtual AAX_Result	PostReleaseRequest( AAX_CParamID  )		{	return AAX_SUCCESS; }
	virtual AAX_Result	PostSetValueRequest( AAX_CParamID , double normalizedValue ) const;
	virtual AAX_Result	GetTouchState ( AAX_CParamID , AAX_CBoolean * outTouched )				{ *outTouched = false; return AAX_SUCCESS; }
	
	//This stuff is to be able to shortcut the automation system.
	void	SetEffectParameters(AAX_IACFEffectParameters* params)											{	mEffectParameters = params; }
	
private:
	AAX_IACFEffectParameters*	mEffectParameters;
};

#endif //UNITTESTAUTOMATIONDELEGATE_H

