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
/*================================================================================================*/
#include "Delay_HostProcessor.h"
#include "string.h"

Delay_HostProcesser::Delay_HostProcesser() 
{
    memset(mDelayLine, 0, sizeof(mDelayLine));  // Initialize the delay line to 0
    rPtr = 0;                                   // Initialize the read pointer
    wPtr = kDelay;                              // Initialize the write pointer
}

Delay_HostProcesser::~Delay_HostProcesser()
{
    
}

// *******************************************************************************
// METHOD:	RenderAudio
// *******************************************************************************
AAX_Result Delay_HostProcesser::RenderAudio ( const float * const inAudioIns [], int32_t inAudioInCount, float * const inAudioOuts [], int32_t inAudioOutCount, int32_t * ioWindowSize )
{
    for ( int channel = 0; channel < inAudioInCount && channel < inAudioOutCount; channel++ )
	{
		const float *	inputBuffer  = inAudioIns [channel];
		float       *	outputBuffer = inAudioOuts [channel];
        
        
		if ( inputBuffer && outputBuffer )
		{
            // Let's create a simple delay
            for ( int i = 0; i < *ioWindowSize; i++ )
            {                
                mDelayLine[wPtr] = inputBuffer[i];      // Fill the delay line at sample wPtr
                outputBuffer [i] = mDelayLine [rPtr];   // Fill the output buffer from the delay line @ sample rPtr
                UpdatePointers(rPtr, wPtr);             // Make sure the pointers don't overrun the length of the delay line
            }
            // End simple delay
        }
        
    }
	return AAX_SUCCESS;
}

// *******************************************************************************
// METHOD:	PostRender
// *******************************************************************************
AAX_Result Delay_HostProcesser::PostRender()
{
    rPtr = 0;       // Re-set the read pointer
    wPtr = kDelay;  // Re-set the write pointer
    
    return AAX_SUCCESS;
}

// *******************************************************************************
// METHOD:	UpdatePointers
// *******************************************************************************
void Delay_HostProcesser::UpdatePointers(int32_t &rPtr, int32_t &wPtr)
{
    wPtr++;
    rPtr++;
    
    if (wPtr >= kMaxDelay)
        wPtr -= kMaxDelay;  // Wrap back around to the beginning of the delay line
    
    if (rPtr >= kMaxDelay)
        rPtr -= kMaxDelay;  // Wrap back around to the beginning of the delay line
        
}
