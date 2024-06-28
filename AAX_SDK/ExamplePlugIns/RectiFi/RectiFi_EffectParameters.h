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
 */

/**  
 *	\file   RectiFi_EffectParameters.h
 *
 *	\brief  RectiFi_EffectParameters class declaration.
 *
 */ 
/*================================================================================================*/ 
#pragma once

#ifndef RECTIFI_EFFECTPARAMETERS_H
#define RECTIFI_EFFECTPARAMETERS_H

#include "AAX_CEffectParameters.h"

// *******************************************************************************
// CLASS:	RectiFi_EffectParameters
// *******************************************************************************
class RectiFi_EffectParameters : public AAX_CEffectParameters
{
public:
	//Constructor
	RectiFi_EffectParameters () : AAX_CEffectParameters() {}
	
	//Overrides from AAX_CEffectParameters
	AAX_Result			EffectInit();

protected:
	AAX_Result	Filt_ProcessFunction( AAX_CParamID inParam, AAX_CPacket& ioPacket );
	AAX_Result	Gain_ProcessFunction( AAX_CPacket& ioPacket );
	AAX_Result	Mix_ProcessFunction( AAX_CPacket& ioPacket );

};

#endif
