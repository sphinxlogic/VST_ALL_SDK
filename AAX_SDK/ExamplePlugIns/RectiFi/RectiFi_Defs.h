/*================================================================================================*/
/*
 *	Copyright 2014-2015 by Avid Technology, Inc.
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
 *	\file   RectiFiDefs.h
 *
 *	\brief  Definitions shared by the RectiFi classes in one easy-to-maintain file.
 */ 
/*================================================================================================*/
#pragma once

#ifndef RECTIFIDEFS_H
#define RECTIFIDEFS_H


/*===================================================================================================*/

// Control index to automation offset
#define kRectiFiControlAutoOffset		-1

// Initial control values
#define kInitialMuteValue				0x80000000		// Off
#define kInitialPreFilterValue			0xC0000000		// less than 200 Hz
#define kInitialRectiTypeValue			0x20000000		// alternating
#define kInitialEffectLevelValue		0x00000000		// 0db
#define kInitialPostFilterValue			0xC0000000		// less than 200 Hz
#define kInitialEffectMixValue			0x00000000		// 50%
#define kInitialBypassValue				0x80000000		// off

// Other local constants:
#define kMasterBypassControl			0
#define kSliderValuePreFilter 			1
#define kButtonValueRectiType 			2
#define kSliderValueEffectLevel			3
#define kSliderValuePostFilter 			4
#define kSliderValueEffectMix			5
#define kNumRectiFiControls 			6

#define kMaxFilterFrequency				(44100.0/2.0)
#define kMinFilterFrequency				(44100.0/1024.0)


#define kPreFilter						0
#define kPostFilter						1

// WARNING : These must remain in sync with the dsp code
#define kPreFilterParamOffset			0
#define kRectiStateParamOffset			9
#define kRectiTypeParamOffset			11
#define kLevelParamOffset				12
#define kPostFilterParamOffset			15
#define kWetMixParamOffset				23
#define kDryMixParamOffset				26
#define kOutputMeterOffset				29
#define kRectiFiParamBlockSize			30

#define kLevelDezipperYOffset			7
#define kWetMixDezipperYOffset			16
#define kDryMixDezipperYOffset			18
#define kRectiFiStateBlockSize			20

#define kNumFilterCoefs					8
#define kMaxChannels					2

#define kCompleteStateChunk		'All '

#define kManufacturerID			'AVID'
#define kProductID				'RcFi'

/*===================================================================================================*/
// Define the ProcessType here
#define kMMProcessTypeID		'11RF'
#define kMMProcessTypeString	"11RF"
	

/*===================================================================================================*/
// Define the ProcessType here
#define kSSProcessTypeID		'22RF'
#define kSSProcessTypeString	"22RF"

/*===================================================================================================*/
// Our RectiFi Plug-In ASMToM program runs on the host CPU.

// Define the ProcessType here
#define kASMToMProcessTypeID	'ASRF'

#define kNativeMToMProcessTypeID	'HMRF'
#define kNativeSToSProcessTypeID	'HSRF'

#define kDPAShimNativeMToMProcessTypeID	'D1RF'
#define kDPAShimNativeSToSProcessTypeID	'D2RF'

#define kTiShimNativeMToMProcessTypeID	'T1RF'
#define kTiShimNativeSToSProcessTypeID	'T2RF'

enum ERectiFi_MaxNumOutputs
{
	kMaxNumInputs = 2,
	kMaxNumOutputs = 2
};

#endif // RECTIFIDEFS_H
