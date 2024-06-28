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
/*================================================================================================*/

#include "DelayComp_HostProcessor.h"
#include "string.h"
#include <algorithm>

DelayComp_HostProcesser::DelayComp_HostProcesser() :
mIsFirstPass(0)
{
    memset(mDelayLine, 0, sizeof(mDelayLine));
    rPtr = 0;
    wPtr = kDelay;
}

DelayComp_HostProcesser::~DelayComp_HostProcesser()
{
    
}

// *******************************************************************************
// METHOD:	PreRender
// *******************************************************************************
AAX_Result DelayComp_HostProcesser::PreRender ( int32_t iAudioInCount, int32_t iAudioOutCount, int32_t iWindowSize )
{
    mIsFirstPass = 1;   

    return AAX_SUCCESS;
}

// *******************************************************************************
// METHOD:	RenderAudio
// *******************************************************************************
AAX_Result DelayComp_HostProcesser::RenderAudio ( const float * const inAudioIns [], int32_t inAudioInCount, float * const inAudioOuts [], int32_t inAudioOutCount, int32_t * ioWindowSize )
{
    
    int32_t j;
    int32_t numSamplesProcessed;
    
	int32_t remainingSamples = (int32_t) ( this->GetDstEnd() - this->GetLocation() );
	int32_t frameSize = std::min( remainingSamples, *ioWindowSize );
    
    
    if (!mIsFirstPass)
        this->GetAudio(inAudioIns, inAudioInCount, (this->GetLocation()+kDelay), &frameSize);
    else
        this->GetAudio(inAudioIns, inAudioInCount, 0, &frameSize);                          
    
	
    for ( int channel = 0; channel < inAudioInCount && channel < inAudioOutCount; channel++ )
	{
		const float *	inputBuffer  = inAudioIns [channel];
		float       *	outputBuffer = inAudioOuts [channel];
        
        
		if ( inputBuffer && outputBuffer )
		{
            
            //Let's create a simple delay
            for ( int i = 0; i < *ioWindowSize; i++ )
            {                
                mDelayLine[wPtr] = inputBuffer[i];
                outputBuffer [i] = mDelayLine [rPtr];
                UpdatePointers(rPtr, wPtr);
            }
            // End simple delay
            
            if (mIsFirstPass)
            {
                j = 0;
                for ( int i = (kDelay); i < (*ioWindowSize); i++ )
                {                    
                    outputBuffer[j] = outputBuffer[i];
                    j++;
                }
                
                numSamplesProcessed = *ioWindowSize - kDelay;
            }
            
                 
        }
        if (mIsFirstPass)
            *ioWindowSize = numSamplesProcessed;
        else
            *ioWindowSize = frameSize;
   
    }
    
   
    
    mIsFirstPass = 0;
        
    
    
	return AAX_SUCCESS;
}

// *******************************************************************************
// METHOD:	PostRender
// *******************************************************************************
AAX_Result DelayComp_HostProcesser::PostRender()
{
    rPtr = 0;
    wPtr = kDelay;
    memset(mDelayLine, 0, sizeof(mDelayLine));
    
    return AAX_SUCCESS;
}

// *******************************************************************************
// METHOD:	UpdatePointers
// *******************************************************************************
void DelayComp_HostProcesser::UpdatePointers(int32_t &rPtr, int32_t &wPtr)
{
    wPtr++;
    rPtr++;
    
    if (wPtr >= kMaxDelay) 
        wPtr -= kMaxDelay;
    
    
    if (rPtr >= kMaxDelay)
        rPtr -= kMaxDelay;
    
}
