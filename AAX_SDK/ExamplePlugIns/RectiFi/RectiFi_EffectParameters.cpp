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
/*================================================================================================*/ 

#include "RectiFi_EffectParameters.h"
#include "RectiFi_Defs.h"
#include "RectiFi_Alg.h"

#include "AAX_CLinearTaperDelegate.h"
#include "AAX_CBinaryTaperDelegate.h"
#include "AAX_CBinaryDisplayDelegate.h"
#include "AAX_CStateTaperDelegate.h"
#include "AAX_CNumberDisplayDelegate.h"
#include "AAX_CStateDisplayDelegate.h"
#include "AAX_CUnitDisplayDelegateDecorator.h"
#include "AAX_SliderConversions.h"

#define RectiFi_ID_PreFilter		"Pre-Filter"
#define RectiFi_ID_Rectification	"Rectification"
#define RectiFi_ID_Gain				"Gain"
#define RectiFi_ID_PostFilter		"Post-Filter"
#define RectiFi_ID_Mix				"Mix"

const char * sRectiFiTypes[] =
{
	"Positive",
	"Negative",
	"Alternate",
	"Alt-Max",
	0
};

// *******************************************************************************
// METHOD:	EffectInit
// *******************************************************************************
AAX_Result RectiFi_EffectParameters::EffectInit()
{	
	// bypass
	AAX_CString bypassID = cDefaultMasterBypassID;
	AAX_IParameter * masterBypass = new AAX_CParameter<bool>(bypassID.CString(), AAX_CString("Master Bypass"), false, AAX_CBinaryTaperDelegate<bool>(), AAX_CBinaryDisplayDelegate<bool>("bypass", "on"), true);
	masterBypass->SetNumberOfSteps( 2 );
	masterBypass->SetType( AAX_eParameterType_Discrete );
	mParameterManager.AddParameter(masterBypass);
	
	AAX_CParameter<float>* parameterPreFilter = new AAX_CParameter<float>( RectiFi_ID_PreFilter, AAX_CString( RectiFi_ID_PreFilter ), 200.0, AAX_CLinearTaperDelegate<float>(kMinFilterFrequency, kMaxFilterFrequency), AAX_CUnitDisplayDelegateDecorator<float>( AAX_CNumberDisplayDelegate<float,0>(), "Hz" ), true);
	parameterPreFilter->SetNumberOfSteps ( 1000 );
	mParameterManager.AddParameter( parameterPreFilter );
	
	AAX_CParameter<int32_t> * parameterRectification = new AAX_CParameter<int32_t>( RectiFi_ID_Rectification, AAX_CString( RectiFi_ID_Rectification ), 1, AAX_CStateTaperDelegate<int32_t>(-1, 2), AAX_CStateDisplayDelegate<int32_t>( sRectiFiTypes, -1 ), true);
	parameterRectification->SetNumberOfSteps ( 4 );
	parameterRectification->SetType ( AAX_eParameterType_Discrete );
	mParameterManager.AddParameter( parameterRectification );
	
	AAX_CParameter<float> * parameterGain = new AAX_CParameter<float>( RectiFi_ID_Gain, AAX_CString( RectiFi_ID_Gain ), 0.0, AAX_CLinearTaperDelegate<float>( -18.0, 18.0), AAX_CUnitDisplayDelegateDecorator<float>( AAX_CNumberDisplayDelegate<float,1>(), "dB" ), true);
	parameterGain->SetNumberOfSteps ( 1000 );
	mParameterManager.AddParameter( parameterGain );

	AAX_CParameter<float> * parameterPostFilter = new AAX_CParameter<float>( RectiFi_ID_PostFilter, AAX_CString( RectiFi_ID_PostFilter ), 200.0, AAX_CLinearTaperDelegate<float>(kMinFilterFrequency, kMaxFilterFrequency), AAX_CUnitDisplayDelegateDecorator<float>( AAX_CNumberDisplayDelegate<float,0>(), "Hz" ), true);
	parameterPostFilter->SetNumberOfSteps ( 1000 );
	mParameterManager.AddParameter( parameterPostFilter );
	
	AAX_CParameter<float> * parameterMix = new AAX_CParameter<float>( RectiFi_ID_Mix, AAX_CString( RectiFi_ID_Mix ), 50.0, AAX_CLinearTaperDelegate<float>(0.0,100.0), AAX_CUnitDisplayDelegateDecorator<float>( AAX_CNumberDisplayDelegate<float,0>(), "%" ), true);
	parameterMix->SetNumberOfSteps ( 1000 );
	mParameterManager.AddParameter( parameterMix );
	
	// register packets
	//
	// one parameter - one packet - one handler
	mPacketDispatcher.RegisterPacket(RectiFi_ID_Gain, eAlgPortID_GainIn, this, &RectiFi_EffectParameters::Gain_ProcessFunction);
	mPacketDispatcher.RegisterPacket(RectiFi_ID_Rectification, eAlgPortID_RectiTypeIn);
	
	// one parameter - two packets - one handler
	mPacketDispatcher.RegisterPacket(RectiFi_ID_PreFilter, eAlgPortID_CoefsPreFiltIn, this, &RectiFi_EffectParameters::Filt_ProcessFunction);
	mPacketDispatcher.RegisterPacket(RectiFi_ID_PostFilter, eAlgPortID_CoefsPostFiltIn, this, &RectiFi_EffectParameters::Filt_ProcessFunction);

	// two parameters - one packet - one handler
	mPacketDispatcher.RegisterPacket(RectiFi_ID_Mix, eAlgPortID_CoefsEffectMixIn, this, &RectiFi_EffectParameters::Mix_ProcessFunction);
	mPacketDispatcher.RegisterPacket(bypassID.CString(), eAlgPortID_CoefsEffectMixIn, this, &RectiFi_EffectParameters::Mix_ProcessFunction);
	
	return AAX_SUCCESS;
}

// *******************************************************************************
// METHOD:	PreFilt_ProcessFunction
// *******************************************************************************
AAX_Result RectiFi_EffectParameters::Filt_ProcessFunction( AAX_CParamID inParam, AAX_CPacket& ioPacket )
{
	AAX_Result result = AAX_ERROR_NULL_OBJECT;

	AAX_IParameter*	 parameter = mParameterManager.GetParameterByID( inParam );
	if (parameter)
	{
		float filterValue;
		if (parameter->GetValueAsFloat(&filterValue))
		{		
			SRectiFiCoefs_Filter*  messagePayloadP = ioPacket.GetPtr<SRectiFiCoefs_Filter>();

			// Retrieve control values
			//
			double cutoffFrequency = filterValue;
			AAX_CSampleRate sampleRate;
			Controller()->GetSampleRate(&sampleRate);
			double omega = (cutoffFrequency / sampleRate * 6.28);
			omega = 2.0 * tan(omega / 2.0);
			double omegaSquared = omega * omega;
			double normalizer = 1.0 / (4.0 + omegaSquared + 2.0 * omega);
			
			// Populate messagePayloadP
			//
			messagePayloadP->mCoeffA2stage1 = (-2.0 * omega + 2.0 + omegaSquared) * normalizer / 4.0;
			messagePayloadP->mCoeffA0stage1 = (2.0 + omegaSquared + 2.0 * omega) * normalizer / 4.0;
			messagePayloadP->mCoeffA1stage1 = (2.0 * omegaSquared - 4.0) * normalizer / 4.0;
			messagePayloadP->mCoeffB2stage1 = (-2.0 * omega + 4.0 + omegaSquared) * normalizer / 2.0;
			messagePayloadP->mCoeffB1stage1 = (2.0 * omegaSquared - 8.0) * normalizer / 2.0;
			messagePayloadP->mCoeffA0stage2 = -2.0 / (2.0 + omega);
			messagePayloadP->mCoeffA1stage2 = 2.0 / (2.0 + omega);
			messagePayloadP->mCoeffB1stage2 = (omega - 2.0) / (2.0 + omega) / 2.0;
			//
			// if max sample rate don't filter
			int32_t controlValue = LogDoubleToLongControl(filterValue, kMinFilterFrequency, kMaxFilterFrequency);
			if (LongControlToNewRange(controlValue, 0, 1000) == 1000)
			{
				// Zero'ing coefficients A0_2 and A1_2 will result in "no filtering"
				messagePayloadP->mCoeffA0stage2 = 0.0;
				messagePayloadP->mCoeffA1stage2 = 0.0;
			}
			
			result = AAX_SUCCESS;
		}
	}
	return result;
}

// *******************************************************************************
// METHOD:	Gain_ProcessFunction
// *******************************************************************************
AAX_Result RectiFi_EffectParameters::Gain_ProcessFunction( AAX_CPacket& ioPacket )
{
	AAX_Result result = AAX_ERROR_NULL_OBJECT;

	AAX_IParameter*	 parameter = mParameterManager.GetParameterByID( RectiFi_ID_Gain );
	if (parameter)
	{
		float gainValue;
		if (parameter->GetValueAsFloat(&gainValue))
		{		
			float*  messagePayloadP = ioPacket.GetPtr<float>();
			*messagePayloadP = pow (10.0f, (gainValue / 20.0f));
			
			result = AAX_SUCCESS;
		}	
	}		
	return result;
}

// *******************************************************************************
// METHOD:	Gain_ProcessFunction
// *******************************************************************************
AAX_Result RectiFi_EffectParameters::Mix_ProcessFunction( AAX_CPacket& ioPacket )
{
	AAX_Result result = AAX_ERROR_NULL_OBJECT;
	
	bool bypass = false;
	AAX_IParameter*	 bypassParam = mParameterManager.GetParameterByID(cDefaultMasterBypassID);
	bypassParam->GetValueAsBool(&bypass);
	
	AAX_IParameter*	 parameter = mParameterManager.GetParameterByID( RectiFi_ID_Mix );
	if (parameter)
	{
		float mixValue;
		if (parameter->GetValueAsFloat(&mixValue))
		{		
			SRectiFiCoefs_EffectMix*  messagePayloadP = ioPacket.GetPtr<SRectiFiCoefs_EffectMix>();
			
			// Retrieve control values
			double effectMix = mixValue;
			effectMix /= 100.0;
			
			// Populate messagePayloadP
			if (bypass)
			{
				messagePayloadP->mDryMix	= 1.0f;
				messagePayloadP->mRectiMix	= 0.0f;
			}
			else if (effectMix < 0.5)
			{
				messagePayloadP->mDryMix = float(1.0f - 0.8f*effectMix);	// [1.0, 0.6] 
				messagePayloadP->mRectiMix = float(1.2f*effectMix);		// [0.0, 0.6]
			}
			else
			{
				messagePayloadP->mDryMix   = float(1.2f - 1.2f*effectMix);	// [0.6, 0.0]
				messagePayloadP->mRectiMix = float(0.2f + 0.8f*effectMix);	// [0.6, 1.0] 
			}
			
			result = AAX_SUCCESS;
		}
	}
	return result;
}
