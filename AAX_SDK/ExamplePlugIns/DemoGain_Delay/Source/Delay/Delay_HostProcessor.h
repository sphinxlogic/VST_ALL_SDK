/*================================================================================================*/
/*
 *	Copyright 2012-2015 by Avid Technology, Inc.
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
 *	\file   Delay_HostProcessor.h
 *
 *	\brief  Definitions shared by the DemoGain_Delay classes in one easy-to-maintain file.
 *
 *  \author Daniel Plazas
 */ 
/*================================================================================================*/
#pragma once
#ifndef DemoGain_Delay_DemoGain_HostProcessor_h
#define DemoGain_Delay_DemoGain_HostProcessor_h

#include "AAX_CHostProcessor.h"
#include "DemoGain_Defs.h"

class Delay_HostProcesser : public AAX_CHostProcessor
{
    
public:
    //Constructor
	Delay_HostProcesser();
    
    ~Delay_HostProcesser();
    
	virtual AAX_Result	RenderAudio ( const float * const inAudioIns [], int32_t inAudioInCount, float * const inAudioOuts [], int32_t inAudioOutCount, int32_t * ioWindowSize );
    virtual AAX_Result	PostRender ();
    
private:
    void        UpdatePointers(int32_t &rPtr, int32_t &wPtr);

    float   mDelayLine[kMaxDelay];   // Arbitrarily large temp buffer
    
    int32_t rPtr;
    int32_t wPtr;
    
};


#endif

