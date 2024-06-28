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

// Component includes
#include "DemoMIDI_Alg.h"

// AAX includes
#include "AAX.h"
#include "AAX_MIDIUtilities.h"

// Standard includes
#include <math.h>		// for fabsf(), sinf()
#include <algorithm>	// for max()

//REM TEST
#include "AAX_Assert.h"



//==============================================================================
// Processing function definition
//==============================================================================

template<int kNumChannelsIn, int kNumChannelsOut> 
void
AAX_CALLBACK
DemoMIDI_AlgorithmProcessFunction (
								   SDemoMIDI_Alg_Context * const	inInstancesBegin [],
								   const void *					inInstancesEnd)
{
	// Get a pointer to the beginning of the memory block table
	SDemoMIDI_Alg_Context* AAX_RESTRICT instance = inInstancesBegin [0];
	
	
	//--------- Iterate over plug-in instances ---------//
	//
	for (SDemoMIDI_Alg_Context * const * walk = inInstancesBegin; walk < inInstancesEnd; ++walk)
	{
		instance = *walk;
		
		//--------- Retrieve instance-specific information ---------//
		//
		// Memory blocks
		const int32_t		bufferSize	= *instance->mBufferSizeP;
		const int32_t		bypass		= *instance->mCtrlBypassP;
		const float			gain		= *instance->mGainP;
		float*				meterTaps	= *instance->mMetersPP;
		SDemoMIDI_NoteBits* const notes	= instance->mIsOn;
		
		// Setup MIDI In node pointers 
		AAX_IMIDINode* midiInNode = instance->mMIDINodeInP;
		const AAX_CMidiStream* midiInStream = midiInNode->GetNodeBuffer();
		const AAX_CMidiPacket* midiInPacketPtr = midiInStream->mBuffer;
		uint32_t inputPacketsRemaining = midiInStream->mBufferSize;
		
		// Setup MIDI Out node pointers
		AAX_IMIDINode* midiOutNode = instance->mMIDINodeOutP;
		AAX_CMidiStream* midiOutStream = midiOutNode->GetNodeBuffer();
		AAX_CMidiPacket* midiOutPacketPtr = midiOutStream->mBuffer;
		midiOutStream->mBufferSize = 0;
		
		//--------- Run processing loop over each channel ---------//
		//
		for (int ch = 0; ch < kNumChannelsIn; ch++)
		{
			
			//--------- Run processing loop over each sample ---------//
			//
			for (int t = 0; t < bufferSize; t++)
			{
				// Set up audio sample pointers
				//
				// Audio channels
				float* const AAX_RESTRICT pdI = instance->mInputPP [0]; // First channel
				float* const AAX_RESTRICT pdO = instance->mOutputPP [0]; // First channel
				
				while ((0 < inputPacketsRemaining) &&
					   (NULL != midiInPacketPtr) &&
					   (midiInPacketPtr->mTimestamp <= t))
				{
					// Handle the MIDI packet
					{
						const uint8_t data1 = midiInPacketPtr->mData[1];
						const uint8_t keySegment = data1/32; // (intentional int division) Segment in SDemoMIDI_NoteBits
						const uint8_t keyOffset = data1 % 32; // Offset in SDemoMIDI_NoteBits
						
						if (AAX::IsNoteOff(midiInPacketPtr))
						{
							const uint32_t mask = ~(0x01 << keyOffset);
							notes->mIsOn[keySegment] &= mask;
						}
						else if (AAX::IsNoteOn(midiInPacketPtr))
						{
							const uint32_t mask = (0x01 << keyOffset);
							notes->mIsOn[keySegment] |= mask;
						}
						else if (AAX::IsAllNotesOff(midiInPacketPtr))
						{
							notes->mIsOn[0] = 0x00000000;
							notes->mIsOn[1] = 0x00000000;
							notes->mIsOn[2] = 0x00000000;
							notes->mIsOn[3] = 0x00000000;
						}
					}
					
					// Just as an example of MIDI output: perform MIDI packet pass-through
					if (NULL != midiOutPacketPtr)
					{
						// Copy the packet from the input MIDI node to the output MIDI node
						memcpy(midiOutPacketPtr, midiInPacketPtr, sizeof(AAX_CMidiPacket));
						
						// Post the MIDI packet
						midiOutNode->PostMIDIPacket(midiOutPacketPtr);
						midiOutStream->mBufferSize++;
					}
					
					// Increment to the next MIDI packet
					++midiInPacketPtr;
					--inputPacketsRemaining;
				}
				
				// Process Audio Samples
				const bool anyNoteIsOn = 0x00 != (notes->mIsOn[0] | notes->mIsOn[1] | notes->mIsOn[2] | notes->mIsOn[3]);
				if (anyNoteIsOn)
				{
					pdO[t] = 1.0f * gain;
				}
				else
				{
					pdO[t] = 0.0f;
				}
				
				// Handle bypass
				if (bypass) 
				{
					pdO[t] = pdI[t];
				}
				
				// Do metering
				//
				// Accumulate the max value for metering. This will get cleared for us by the shell
				// when it sends the accumulated value up to the host.
				meterTaps[eMeterTap_PreGain] = std::max(fabsf(instance->mInputPP [0] [t]), meterTaps[eMeterTap_PreGain]);
				meterTaps[eMeterTap_PostGain] = std::max(fabsf(instance->mOutputPP [0] [t]), meterTaps[eMeterTap_PostGain]);
				
			} // Go to the next sample
		} // Go to next channel
	} // End instance-iteration loop
}
