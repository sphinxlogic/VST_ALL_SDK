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
/*================================================================================================*/

// RectiFi includes
#include "RectiFi_Alg.h"

// AAX includes
#include "AAX.h"


//==============================================================================
// Processing function definition
//==============================================================================

template<int kNumChannelsIn, int kNumChannelsOut, int kAudioWindowSize>
void AAX_CALLBACK RectiFi_Algorithm_ProcessFunction(
	SRectiFi_Algorithm_Context * const	inInstancesBegin [],
	const void *						inInstancesEnd)
{

	//--------- Iterate over plug-in instances ---------//
#pragma MUST_ITERATE(1)
	for (SRectiFi_Algorithm_Context * const * walk = inInstancesBegin; walk != inInstancesEnd; ++walk)
	{
		SRectiFi_Algorithm_Context* const AAX_RESTRICT contextP = *walk;

		//--------- Retrieve instance-specific information ---------//
		//
		// Memory blocks
		const SRectiFiCoefs_EffectMix* const AAX_RESTRICT	coefsEffectMixP =	contextP->mCoefsEffectMixP;	// Input (const)
		const SRectiFiCoefs_Filter* const AAX_RESTRICT		coefsPreFiltP =		contextP->mCoefsPreFiltP;	// Input (const)
		const SRectiFiCoefs_Filter* const AAX_RESTRICT		coefsPostFiltP =	contextP->mCoefsPostFiltP;	// Input (const)
		SRectiFiStateBlock* const AAX_RESTRICT				stateP =			contextP->mStateP;			// Private (non-const)
		//
		// Coefficients stored as local copies
		const int32_t									rectiType =			*contextP->mCtrlRectiTypeP;
		const float										gain =				*contextP->mGainP;

#ifdef _TMS320C6X
		const int32_t	numSamples = kAudioWindowSize;
#else
		const int32_t	numSamples = *contextP->mBufferLengthP;
#endif			
		
		const int32_t rectiTypeChanged = (rectiType != stateP->mRectiType);
		stateP->mRectiType = rectiType;

		for ( int channel = 0; channel < kNumChannelsIn; ++channel)
			stateP->mRectiPositive[channel] = (rectiTypeChanged * (rectiType != 0)) + ((!rectiTypeChanged) * stateP->mRectiPositive[channel]);

		//--------- Run processing loop over each input channel ---------//
		//
		for (int i = 0; i < kNumChannelsIn; i++) // Iterate over all input channels
		{
			//--------- Run processing loop over each sample ---------//
			//
			const	float*	const AAX_RESTRICT	inputsP = contextP->mInputPP[i]; 
					float*	const AAX_RESTRICT	outputsP = contextP->mOutputPP[i]; 

			double	DelayPreA1stage1 = stateP->mDelayPreA1stage1[i];
			double	DelayPreA2stage1 = stateP->mDelayPreA2stage1[i];
			double	DelayPreB1stage1 = stateP->mDelayPreB1stage1[i];
			double	DelayPreB2stage1 = stateP->mDelayPreB2stage1[i];
			double	DelayPreA1stage2 = stateP->mDelayPreA1stage2[i];
			double	DelayPreB1stage2 = stateP->mDelayPreB1stage2[i];

#pragma UNROLL(kAudioWindowSize)
			for (int t = 0; t < numSamples; t++)
			{
				outputsP[t] = inputsP[t];		//output used for intermediates.

				//*************************************** Do Pre-Filter ************************************
				double thePreFilterSum;
				double thePreFilterOutput;
				
				thePreFilterSum	=	outputsP[t]	* coefsPreFiltP->mCoeffA0stage1;
				thePreFilterSum	+=	DelayPreA1stage1	* coefsPreFiltP->mCoeffA1stage1;
				thePreFilterSum	+=	DelayPreA2stage1	* coefsPreFiltP->mCoeffA2stage1;
				
				thePreFilterSum	+=	-DelayPreB1stage1	* coefsPreFiltP->mCoeffB1stage1;
				thePreFilterSum	+=	-DelayPreB2stage1	* coefsPreFiltP->mCoeffB2stage1;
				thePreFilterSum	=	thePreFilterSum		* 2.0f;

				thePreFilterOutput	=	thePreFilterSum		* coefsPreFiltP->mCoeffA0stage2;
				thePreFilterOutput	+=	DelayPreA1stage2	* coefsPreFiltP->mCoeffA1stage2;
				thePreFilterOutput	+=	-DelayPreB1stage2	* coefsPreFiltP->mCoeffB1stage2;
				thePreFilterOutput	=	thePreFilterOutput	* 2.0f;

				DelayPreA2stage1 = DelayPreA1stage1;
				DelayPreA1stage1 = outputsP[t];
				DelayPreB2stage1 = DelayPreB1stage1;
				DelayPreB1stage1 = thePreFilterSum;
				DelayPreA1stage2 = thePreFilterSum;
				DelayPreB1stage2 = thePreFilterOutput;

				thePreFilterOutput	+=	outputsP[t] * 0.5f;
				outputsP[t]	=	static_cast<float>(thePreFilterOutput) * 2.0f;
			}

			stateP->mDelayPreA1stage1[i] = DelayPreA1stage1;
			stateP->mDelayPreA2stage1[i] = DelayPreA2stage1;
			stateP->mDelayPreB1stage1[i] = DelayPreB1stage1;
			stateP->mDelayPreB2stage1[i] = DelayPreB2stage1;
			stateP->mDelayPreA1stage2[i] = DelayPreA1stage2;
			stateP->mDelayPreB1stage2[i] = DelayPreB1stage2;



			int32_t rectiPositive = stateP->mRectiPositive[i];

#pragma UNROLL(kAudioWindowSize)
			for (int t = 0; t < numSamples; t++)
			{
				//*************************************** Do Rectify ************************************
				
				if (rectiType >= 1)			//"alternating" and "alt-max"
				{
					if ((stateP->mRectiLastInput[i] < 0.0) && (outputsP[t] >= 0.0))
						rectiPositive = !rectiPositive;

					stateP->mRectiLastInput[i] = outputsP[t];
				}

				if (( rectiPositive && outputsP[t] < 0.0) 
					|| (!rectiPositive && outputsP[t] > 0.0))
				{
					outputsP[t] = -outputsP[t];
				}

				if (rectiType == 2)		//"alt-max"
				{
					if ( (rectiPositive && (outputsP[t] < stateP->mRectiLastOutput[i]))
						|| (!rectiPositive && (outputsP[t] > stateP->mRectiLastOutput[i])) )
						outputsP[t] = stateP->mRectiLastOutput[i];

					stateP->mRectiLastOutput[i] = outputsP[t];
				}

				//*************************************** Do Gain Stage *************************************

				outputsP[t] = outputsP[t] * gain;
			}
			stateP->mRectiPositive[i] = rectiPositive;


			double	DelayPostA1stage1 = stateP->mDelayPostA1stage1[i];
			double	DelayPostA2stage1 = stateP->mDelayPostA2stage1[i];
			double	DelayPostB1stage1 = stateP->mDelayPostB1stage1[i];
			double	DelayPostB2stage1 = stateP->mDelayPostB2stage1[i];
			double	DelayPostA1stage2 = stateP->mDelayPostA1stage2[i];
			double	DelayPostB1stage2 = stateP->mDelayPostB1stage2[i];

#pragma UNROLL(kAudioWindowSize)
			for (int t = 0; t < numSamples; t++)
			{
				//*************************************** Do Post-Filter ************************************
				double thePostFilterSum;
				double thePostFilterOutput;

				thePostFilterSum	=	outputsP[t]	* coefsPostFiltP->mCoeffA0stage1;
				thePostFilterSum	+=	DelayPostA1stage1	* coefsPostFiltP->mCoeffA1stage1;
				thePostFilterSum	+=	DelayPostA2stage1	* coefsPostFiltP->mCoeffA2stage1;

				thePostFilterSum	+=	-DelayPostB1stage1	* coefsPostFiltP->mCoeffB1stage1;
				thePostFilterSum	+=	-DelayPostB2stage1	* coefsPostFiltP->mCoeffB2stage1;
				thePostFilterSum	=	thePostFilterSum	* 2.0f;

				thePostFilterOutput	=	thePostFilterSum	* coefsPostFiltP->mCoeffA0stage2;
				thePostFilterOutput	+=	DelayPostA1stage2	* coefsPostFiltP->mCoeffA1stage2;
				thePostFilterOutput +=	-DelayPostB1stage2	* coefsPostFiltP->mCoeffB1stage2;
				thePostFilterOutput	=	thePostFilterOutput	* 2.0f;

				DelayPostA2stage1 = DelayPostA1stage1;
				DelayPostA1stage1 = outputsP[t];
				DelayPostB2stage1 = DelayPostB1stage1;
				DelayPostB1stage1 = thePostFilterSum;
				DelayPostA1stage2 = thePostFilterSum;
				DelayPostB1stage2 = thePostFilterOutput;

				thePostFilterOutput	+=	outputsP[t] * 0.5f;
				outputsP[t]	=	static_cast<float>(thePostFilterOutput) * 2.0f;

				const float drySample = inputsP[t];
				//****************************************** Do Mix *****************************************

				outputsP[t] = (drySample * coefsEffectMixP->mDryMix) + (outputsP[t] * coefsEffectMixP->mRectiMix);

				//********************************** Send output on its way *********************************
				//NOTE: bypass is processed in Mix component
				//outputsP[t] = (float (bypass) * drySample) + (float (!bypass) * currentSample[t]);

			} // Go to next sample time

			stateP->mDelayPostA1stage1[i] = DelayPostA1stage1;
			stateP->mDelayPostA2stage1[i] = DelayPostA2stage1;
			stateP->mDelayPostB1stage1[i] = DelayPostB1stage1;
			stateP->mDelayPostB2stage1[i] = DelayPostB2stage1;
			stateP->mDelayPostA1stage2[i] = DelayPostA1stage2;
			stateP->mDelayPostB1stage2[i] = DelayPostB1stage2;


		} // Go to the next channel
	} // End instance-iteration loop
}

