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
 *	\file   DemoGain_Defs.h
 *
 *	\brief  Definitions shared by the DemoGain_Delay classes in one easy-to-maintain file.
 */ 
/*================================================================================================*/

#pragma once
#ifndef DEMOGAIN_DEFS_H
#define DEMOGAIN_DEFS_H

#include "AAX.h"			// for AAX_CTypeID


// Type, product, and relation IDs
const AAX_CTypeID cDemoGainDelay_ManufactureID              = 'AVID';
const AAX_CTypeID cDemoGainDelay_ProductID                  = 'DmGd';
const AAX_CTypeID cDemoGainDelay_PlugInID_AudioSuite        = 'DGDd';
const AAX_CTypeID cDemoGainDelayComp_PlugInID_AudioSuite    = 'DGDc';

const AAX_CTypeID cDemoGain_MeterID [2] =			{'mtrI','mtrO'};

#define kDelay              2048 // Let's create a delay of 2048 samples
#define kMaxDelay           3000 // Arbitrary maximum sample delay

#endif // DEMOGAIN_DEFS_H
