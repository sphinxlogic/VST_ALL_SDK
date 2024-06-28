/*================================================================================================*/
/*
 *	Copyright 2013-2015 by Avid Technology, Inc.
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
 *	\file   DemoMIDI_Alg.h
 *
 *	\brief  Algorithm component definitions for DemoDelay.
 *
 *  \author Danny Plazas
 */ 
/*================================================================================================*/
#pragma once
#ifndef DEMOMIDI_ALG_H
#define DEMOMIDI_ALG_H



// AAX includes
#include "AAX.h"
#include "AAX_IMIDINode.h"

//==============================================================================
// General definitions
//==============================================================================

// Meter layout
enum EDemoMIDI_MeterTaps
{
	eMeterTap_PreGain = 0,
	eMeterTap_PostGain,
	
	eMeterTap_Count
};


//==============================================================================
// Component context definitions
//==============================================================================

#include AAX_ALIGN_FILE_ALG

struct SDemoMIDI_NoteBits
{
	uint32_t mIsOn[4]; // bits for 128 note values
};

// Context structure
struct SDemoMIDI_Alg_Context
{
	int32_t						* mCtrlBypassP;					// Control message destination
	float						* mGainP;						// Coefficient message destination
	
	AAX_IMIDINode				* mMIDINodeInP;					// Buffered local MIDI input node Pointer
	AAX_IMIDINode				* mMIDINodeOutP;				// Buffered local MIDI output node Pointer
	
	SDemoMIDI_NoteBits			* mIsOn;						// Private data
	
	float*						* mInputPP;						// Audio signal destination
	float*						* mOutputPP;					// Audio signal source
	int32_t						* mBufferSizeP;					// Buffer size
	float*						* mMetersPP;					// Meter taps
};

#include AAX_ALIGN_FILE_RESET


// Physical addresses within the context
enum EDemoMIDI_Alg_PortID
{
	eAlgPortID_BypassIn				= AAX_FIELD_INDEX (SDemoMIDI_Alg_Context, mCtrlBypassP)
	,eAlgPortID_CoefsGainIn			= AAX_FIELD_INDEX (SDemoMIDI_Alg_Context, mGainP)
	,eAlgPortID_MIDINodeIn			= AAX_FIELD_INDEX (SDemoMIDI_Alg_Context, mMIDINodeInP)
	,eAlgPortID_MIDINodeOut			= AAX_FIELD_INDEX (SDemoMIDI_Alg_Context, mMIDINodeOutP)
	,eAlgPortID_IsOn				= AAX_FIELD_INDEX (SDemoMIDI_Alg_Context, mIsOn)
	,eAlgFieldID_AudioIn			= AAX_FIELD_INDEX (SDemoMIDI_Alg_Context, mInputPP)
	,eAlgFieldID_AudioOut			= AAX_FIELD_INDEX (SDemoMIDI_Alg_Context, mOutputPP)
	,eAlgFieldID_BufferSize			= AAX_FIELD_INDEX (SDemoMIDI_Alg_Context, mBufferSizeP)
	,eAlgFieldID_Meters				= AAX_FIELD_INDEX (SDemoMIDI_Alg_Context, mMetersPP)
};


//==============================================================================
// Callback declarations
//==============================================================================
/*
 void
 AAX_CALLBACK
 DemoMIDI_AlgorithmProcessFunction (
 SDemoMIDI_Alg_Context * const	inInstancesBegin [],
 const void *					inInstancesEnd);
 */

#endif
