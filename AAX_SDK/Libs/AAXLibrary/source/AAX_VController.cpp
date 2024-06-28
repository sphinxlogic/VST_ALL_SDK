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
 *
 */
/*================================================================================================*/

#include "AAX_VController.h"
#include "AAX_UIDs.h"

// ******************************************************************************************
// METHOD:	AAX_VController
// ******************************************************************************************
AAX_VController::AAX_VController( IACFUnknown* pUnknown )
{
	if ( pUnknown )
	{
		pUnknown->QueryInterface(IID_IAAXControllerV1, (void **)&mIController);
		pUnknown->QueryInterface(IID_IAAXControllerV2, (void **)&mIControllerV2);
		pUnknown->QueryInterface(IID_IAAXControllerV3, (void **)&mIControllerV3);
	}	
}

// ******************************************************************************************
// METHOD:	~AAX_VController
// ******************************************************************************************
AAX_VController::~AAX_VController()
{
}

// ******************************************************************************************
// METHOD:	PostPacket
// ******************************************************************************************
AAX_Result AAX_VController::PostPacket ( AAX_CFieldIndex inFieldIndex, const void * inPayloadP, uint32_t inPayloadSize )
{
	if ( mIController )
		return mIController->PostPacket ( inFieldIndex, inPayloadP, inPayloadSize );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	SendNotification
// ******************************************************************************************
AAX_Result AAX_VController::SendNotification (AAX_CTypeID iNotificationType, const void* iNotificationData, uint32_t iNotificationDataSize)
{
	if (mIControllerV2)
		return mIControllerV2->SendNotification(iNotificationType, iNotificationData, iNotificationDataSize);

	return AAX_ERROR_UNIMPLEMENTED;
}

// ******************************************************************************************
// METHOD:	GetEffectID
// ******************************************************************************************
AAX_Result	AAX_VController::GetEffectID ( AAX_IString *	outEffectID) const
{
	if (mIController)
		return mIController->GetEffectID(outEffectID);

	return AAX_ERROR_NULL_OBJECT;
}


// ******************************************************************************************
// METHOD:	GetEffectSampleRate
// ******************************************************************************************
AAX_Result AAX_VController::GetSampleRate ( AAX_CSampleRate * oSampleRate ) const
{
	if ( mIController )
		return mIController->GetSampleRate ( oSampleRate );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	GetEffectInputStemFormat
// ******************************************************************************************
AAX_Result AAX_VController::GetInputStemFormat ( AAX_EStemFormat * oStemFormat ) const
{
	if ( mIController )
		return mIController->GetInputStemFormat ( oStemFormat );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	GetEffectOutputStemFormat
// ******************************************************************************************
AAX_Result AAX_VController::GetOutputStemFormat ( AAX_EStemFormat * oStemFormat ) const
{
	if ( mIController )
		return mIController->GetOutputStemFormat ( oStemFormat );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	GetTODLocation
// ******************************************************************************************
AAX_Result AAX_VController::GetTODLocation ( AAX_CTimeOfDay* oTODLocation ) const
{
	if ( mIController )
		return mIController->GetTODLocation ( oTODLocation );

	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	GetCurrentAutomationTimestamp
// ******************************************************************************************
AAX_Result  AAX_VController::GetCurrentAutomationTimestamp(AAX_CTransportCounter* oTimestamp) const
{
    if (mIControllerV2)
        return mIControllerV2->GetCurrentAutomationTimestamp(oTimestamp);
    
    *oTimestamp = 0;
    return AAX_ERROR_UNIMPLEMENTED;
}


// ******************************************************************************************
// METHOD:	GetSignalLatency
// ******************************************************************************************
AAX_Result	AAX_VController::GetSignalLatency( int32_t* outSamples) const
{
	if (mIController )
		return mIController->GetSignalLatency( outSamples );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	GetHybridSignalLatency
// ******************************************************************************************
AAX_Result AAX_VController::GetHybridSignalLatency(int32_t* oSamples) const
{
	if (mIControllerV2 )
		return mIControllerV2->GetHybridSignalLatency( oSamples );
	
	return AAX_ERROR_UNIMPLEMENTED;
}

// ******************************************************************************************
// METHOD:	SetSignalLatency
// ******************************************************************************************
AAX_Result	AAX_VController::SetSignalLatency(int32_t numSamples)
{
	if (mIController )
		return mIController->SetSignalLatency( numSamples );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	GetCycleCount
// ******************************************************************************************
AAX_Result	AAX_VController::GetCycleCount( AAX_EProperty inWhichCycleCount, AAX_CPropertyValue* outNumCycles) const
{
	if (mIController )
		return mIController->GetCycleCount( inWhichCycleCount, outNumCycles );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	SetCycleCount
// ******************************************************************************************
AAX_Result	AAX_VController::SetCycleCount( AAX_EProperty* inWhichCycleCounts, AAX_CPropertyValue* inValues, int32_t inNumValues)
{
	if (mIController )
		return mIController->SetCycleCount( inWhichCycleCounts, inValues, inNumValues );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	GetCurrentMeterValue
// ******************************************************************************************
AAX_Result	AAX_VController::GetCurrentMeterValue( AAX_CTypeID inMeterID, float * outMeterValue ) const
{
	if ( mIController )
		return mIController->GetCurrentMeterValue ( inMeterID, outMeterValue );

	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	GetMeterPeakValue
// ******************************************************************************************
AAX_Result	AAX_VController::GetMeterPeakValue( AAX_CTypeID inMeterID, float * outMeterPeakValue ) const
{
	if ( mIController )
		return mIController->GetMeterPeakValue ( inMeterID, outMeterPeakValue );

	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	ClearMeterPeakValue
// ******************************************************************************************
AAX_Result	AAX_VController::ClearMeterPeakValue( AAX_CTypeID inMeterID ) const
{
	if ( mIController )
		return mIController->ClearMeterPeakValue ( inMeterID );

	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	GetMeterClipped
// ******************************************************************************************
AAX_Result	AAX_VController::GetMeterClipped( AAX_CTypeID inMeterID, AAX_CBoolean * outClipped ) const
{
	if ( mIController )
		return mIController->GetMeterClipped ( inMeterID, outClipped );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	ClearMeterClipped
// ******************************************************************************************
AAX_Result	AAX_VController::ClearMeterClipped( AAX_CTypeID inMeterID ) const
{
	if ( mIController )
		return mIController->ClearMeterClipped ( inMeterID );
	
	return AAX_ERROR_NULL_OBJECT;
}


// ******************************************************************************************
// METHOD:	GetMeterCount
// ******************************************************************************************
AAX_Result	AAX_VController::GetMeterCount( uint32_t * outMeterCount ) const
{
	if ( mIController )
		return mIController->GetMeterCount ( outMeterCount );
	
	return AAX_ERROR_NULL_OBJECT;
}

// ******************************************************************************************
// METHOD:	GetNextMIDIPacket
// ******************************************************************************************
AAX_Result AAX_VController::GetNextMIDIPacket( AAX_CFieldIndex* outPort, AAX_CMidiPacket* outPacket )
{
	if ( mIController )
		return mIController->GetNextMIDIPacket( outPort, outPacket );

	return AAX_ERROR_NULL_OBJECT;
}

AAX_Result AAX_VController::GetPlugInTargetPlatform(AAX_CTargetPlatform* oTargetPlatform) const
{
	if (mIControllerV3)
		return mIControllerV3->GetPlugInTargetPlatform(oTargetPlatform);

	return AAX_ERROR_UNIMPLEMENTED;
}

AAX_Result AAX_VController::GetIsAudioSuite(AAX_CBoolean* oIsAudioSuite) const
{
	if (mIControllerV3)
		return mIControllerV3->GetIsAudioSuite(oIsAudioSuite);
	
	return AAX_ERROR_UNIMPLEMENTED;
}

AAX_Result AAX_VController::GetHostName(AAX_IString* oHostNameString) const
{
    if (mIControllerV2)
        return mIControllerV2->GetHostName(oHostNameString);
    return AAX_ERROR_UNIMPLEMENTED;
}


