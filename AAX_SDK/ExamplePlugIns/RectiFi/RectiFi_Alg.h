/*================================================================================================*/
/*
 *	Copyright 2008-2015 by Avid Technology, Inc.
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
 *	\file   RectiFi_Alg.h
 *
 *	\brief  Algorithm component definitions for DemoDelay.
 *
 *  \author Frederick Umminger
 */ 
/*================================================================================================*/
#pragma once

#ifndef RECTIFI_ALG_H
#define RECTIFI_ALG_H


// RectiFi includes
#include "RectiFi_Defs.h"

// DPA includes
#include "AAX.h"

class IACFUnknown;

#include AAX_ALIGN_FILE_ALG

//==============================================================================
// Memory block structure definitions
//==============================================================================

struct SRectiFiCoefs_Filter
{
	double	mCoeffA0stage1;
	double	mCoeffA1stage1;
	double	mCoeffA2stage1;
	double	mCoeffB1stage1;
	double	mCoeffB2stage1;
	double	mCoeffA0stage2;
	double	mCoeffA1stage2;
	double	mCoeffB1stage2;
};

struct SRectiFiStateBlock
{
	double 	mDelayPreA1stage1[kMaxNumOutputs];
	double	mDelayPreA2stage1[kMaxNumOutputs];
	double	mDelayPreB1stage1[kMaxNumOutputs];
	double	mDelayPreB2stage1[kMaxNumOutputs];
	double	mDelayPreA1stage2[kMaxNumOutputs];
	double	mDelayPreB1stage2[kMaxNumOutputs];
	
	double	mDelayPostA1stage1[kMaxNumOutputs];
	double	mDelayPostA2stage1[kMaxNumOutputs];
	double	mDelayPostB1stage1[kMaxNumOutputs];
	double	mDelayPostB2stage1[kMaxNumOutputs];
	double	mDelayPostA1stage2[kMaxNumOutputs];
	double	mDelayPostB1stage2[kMaxNumOutputs];

	float	mRectiLastInput[kMaxNumOutputs];
	float	mRectiLastOutput[kMaxNumOutputs];
	int		mRectiType;
	int		mRectiPositive[kMaxNumOutputs];
};

struct SRectiFiCoefs_EffectMix
{
	float	mDryMix;
	float	mRectiMix;
};

//==============================================================================
// Component context definitions
//==============================================================================

// Context structure
struct SRectiFi_Algorithm_Context
{
	const float * const					* mInputPP;					// Audio signal destination
	float * const						* mOutputPP;				// Audio signal source
	const int32_t						* mBufferLengthP;			
	
	const int32_t						* mCtrlRectiTypeP;			// Control message destination

	const float							* mGainP;					// Inter-component message destination
	const SRectiFiCoefs_Filter			* mCoefsPreFiltP;			// Inter-component message destination
	const SRectiFiCoefs_Filter			* mCoefsPostFiltP;			// Inter-component message destination
	const SRectiFiCoefs_EffectMix		* mCoefsEffectMixP;			// Inter-component message destination

	SRectiFiStateBlock					* mStateP;					// Private data
};

// Physical addresses within the context
enum ERectiFi_AlgPortID
{
	eAlgFieldIndex_AudioIn				= AAX_FIELD_INDEX (SRectiFi_Algorithm_Context, mInputPP),
	eAlgFieldIndex_AudioOut				= AAX_FIELD_INDEX (SRectiFi_Algorithm_Context, mOutputPP),
	eAlgFieldIndex_BufferLength			= AAX_FIELD_INDEX (SRectiFi_Algorithm_Context, mBufferLengthP),

	eAlgPortID_RectiTypeIn				= AAX_FIELD_INDEX (SRectiFi_Algorithm_Context, mCtrlRectiTypeP),

	eAlgPortID_GainIn					= AAX_FIELD_INDEX (SRectiFi_Algorithm_Context, mGainP),
	eAlgPortID_CoefsPreFiltIn			= AAX_FIELD_INDEX (SRectiFi_Algorithm_Context, mCoefsPreFiltP),
	eAlgPortID_CoefsPostFiltIn			= AAX_FIELD_INDEX (SRectiFi_Algorithm_Context, mCoefsPostFiltP),
	eAlgPortID_CoefsEffectMixIn			= AAX_FIELD_INDEX (SRectiFi_Algorithm_Context, mCoefsEffectMixP),

	eAlgFieldIndex_State				= AAX_FIELD_INDEX (SRectiFi_Algorithm_Context, mStateP)
};

// Function declaration
void AAX_CALLBACK RectiFi_Algorithm_InitStateBlock(
	AAX_CFieldIndex,
	void *			inNewBlock,
	int32_t,
	IACFUnknown * const);


#include AAX_ALIGN_FILE_RESET

#endif // RECTIFI_ALG_H
